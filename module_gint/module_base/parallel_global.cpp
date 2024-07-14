//==========================================================
// AUTHOR : fangwei, mohan
// DATE : 2009-11-08
//==========================================================
#include "parallel_global.h"

#ifdef __MPI
#include <mpi.h>
#endif
#ifdef _OPENMP
#include <omp.h>
#endif

#include "global_function.h"
#include "parallel_common.h"
#include "parallel_reduce.h"
#include "version.h"

#include <iostream>
#include <thread>

#if defined __MPI
MPI_Comm POOL_WORLD;
MPI_Comm INTER_POOL = MPI_COMM_NULL;
MPI_Comm STO_WORLD;
MPI_Comm PARAPW_WORLD; // qianrui add it for sto-dft 2021-4-14
MPI_Comm GRID_WORLD;   // mohan add 2012-01-13z
MPI_Comm DIAG_WORLD;   // mohan add 2012-01-13

namespace Parallel_Global
{
int mpi_number = 0;
int omp_number = 0;
} // namespace Parallel_Global

void Parallel_Global::myProd(std::complex<double>* in, std::complex<double>* inout, int* len, MPI_Datatype* dptr)
{
    for (int i = 0; i < *len; i++)
    {
        //		(*inout).real()=(*inout).real()+(*in).real();
        //		(*inout).imag()=(*inout).imag()+(*in).imag();

        // mohan updat 2011-09-21
        (*inout) = std::complex<double>((*inout).real() + (*in).real(), (*inout).imag() + (*in).imag());

        in++;
        inout++;
    }
    return;
}
#endif

void Parallel_Global::split_diag_world(const int& diag_np,
                                       const int& nproc,
                                       const int& my_rank,
                                       int& drank,
                                       int& dsize,
                                       int& dcolor)
{
#ifdef __MPI
    assert(diag_np > 0);
    int group_grid_np = -1;
    int color = -1;
    int key = -1;
    divide_mpi_groups(nproc, diag_np, my_rank, group_grid_np, key, color);
    MPI_Comm_split(MPI_COMM_WORLD, color, key, &DIAG_WORLD);
    MPI_Comm_rank(DIAG_WORLD, &drank);
    MPI_Comm_size(DIAG_WORLD, &dsize);
    dcolor = color;
#else
    dcolor = 0; // mohan fix bug 2012-02-04
    drank = 0;
    dsize = 1;
#endif
    return;
}

void Parallel_Global::split_grid_world(const int diag_np, const int& nproc, const int& my_rank, int& grank, int& gsize)
{
#ifdef __MPI
    assert(diag_np > 0);
    int group_grid_np = -1;
    int color = -1;
    int key = -1;
    divide_mpi_groups(nproc, diag_np, my_rank, group_grid_np, color, key);
    MPI_Comm_split(MPI_COMM_WORLD, color, key, &GRID_WORLD);
    MPI_Comm_rank(GRID_WORLD, &grank);
    MPI_Comm_size(GRID_WORLD, &gsize);
#else
    grank = 0; // mohan fix bug 2012-02-04
    gsize = 1;
#endif
    return;
}

void Parallel_Global::read_mpi_parameters(int argc, char** argv, int& NPROC, int& MY_RANK)
{
#ifdef __MPI
#ifdef _OPENMP
    int provided = 0;
    MPI_Init_thread(&argc, &argv, MPI_THREAD_MULTIPLE, &provided);
    if (provided != MPI_THREAD_MULTIPLE)
    {
        std::cerr << "MPI_Init_thread request " << MPI_THREAD_MULTIPLE << " but provide " << provided << std::endl;
    }
    // Peize Lin change 2022.08.08
    // MPI_THREAD_FUNNELED is enough for ABACUS. Using MPI_THREAD_SERIALIZED for elpa, using MPI_THREAD_MULTIPLE for
    // libRI.
#else
    MPI_Init(&argc, &argv); // Peize Lin change 2018-07-12
#endif //_OPENMP

    //  KPAR = atoi(argv[1]); // mohan abandon 2010-06-09

    // get world size --> NPROC
    // get global rank --> MY_RANK
    MPI_Comm_size(MPI_COMM_WORLD, &NPROC);
    MPI_Comm_rank(MPI_COMM_WORLD, &MY_RANK);
    int process_num = 0; // number of processes in the current node
    int local_rank = 0;  // rank of the process in the current node
    MPI_Comm shmcomm;
    MPI_Comm_split_type(MPI_COMM_WORLD, MPI_COMM_TYPE_SHARED, 0, MPI_INFO_NULL, &shmcomm);
    MPI_Comm_size(shmcomm, &process_num);
    MPI_Comm_rank(shmcomm, &local_rank);
    MPI_Comm_free(&shmcomm);

    // Determining appropriate thread number for OpenMP:
    // 1. If the number of threads is set by the user by `OMP_NUM_THREADS`, use it.
    // 2. Otherwise, set to number of CPU cores / number of processes.
    // 3. If the number of threads is larger than the hardware availability (should only happens if route 1 taken),
    //  output a warning message.
    // 4. If the number of threads is smaller than the hardware availability, output an info message.
    // CAVEAT: The user should set the number of threads properly to avoid oversubscribing.
    // This mechanism only handles the worst case for the default setting (not setting number of threads at all, causing
    // oversubscribing and extremely slow performance), not guaranteed to be optimal.
    const int max_thread_num = std::thread::hardware_concurrency(); // Consider Hyperthreading disabled.
#ifdef _OPENMP
    int current_thread_num = omp_get_max_threads(); // Get the number of threads set by the user.
    if (current_thread_num == max_thread_num
        && process_num >= 1) // Avoid oversubscribing on the number of threads not set.
    {
        current_thread_num = max_thread_num / process_num;
        omp_set_num_threads(current_thread_num);
    }
#else
    int current_thread_num = 1;
#endif
    mpi_number = process_num;
    omp_number = current_thread_num;
    if (current_thread_num * process_num > max_thread_num && local_rank == 0)
    {
        std::stringstream mess;
        mess << "WARNING: Total thread number(" << current_thread_num * process_num << ") "
             << "is larger than hardware availability(" << max_thread_num << ")." << std::endl
             << "The results may be INCORRECT. Please set the environment variable OMP_NUM_THREADS to a proper value."
             << std::endl;
        std::cerr << mess.str() << std::endl;
        // the user may take their own risk by set the OMP_NUM_THREADS env var.
        if (std::getenv("OMP_NUM_THREADS") == nullptr)
        {
            exit(1);
        }
    }
    else if (current_thread_num * process_num < max_thread_num && local_rank == 0)
    {
        // only output info in local rank 0
        std::cerr << "Info: Local MPI proc number: " << process_num << ","
                  << "OpenMP thread number: " << current_thread_num << ","
                  << "Total thread number: " << current_thread_num * process_num << ","
                  << "Local thread limit: " << max_thread_num << std::endl;
    }

    if (MY_RANK == 0)
    {
#ifdef VERSION
        const char* version = VERSION;
#else
        const char* version = "unknown";
#endif
#ifdef COMMIT_INFO
#include "commit.h"
        const char* commit = COMMIT;
#else
        const char* commit = "unknown";
#endif
        std::cout << "                                                                                     "
                  << std::endl
                  << "                              ABACUS " << version << std::endl
                  << std::endl
                  << "               Atomic-orbital Based Ab-initio Computation at UStc                    "
                  << std::endl
                  << std::endl
                  << "                     Website: http://abacus.ustc.edu.cn/                             "
                  << std::endl
                  << "               Documentation: https://abacus.deepmodeling.com/                       "
                  << std::endl
                  << "                  Repository: https://github.com/abacusmodeling/abacus-develop       "
                  << std::endl
                  << "                              https://github.com/deepmodeling/abacus-develop         "
                  << std::endl
                  << "                      Commit: " << commit << std::endl
                  << std::endl;
        time_t time_now = time(NULL);
        std::cout << " " << ctime(&time_now);
    }

    // for test
    /*
    for (int i=0; i<NPROC; i++)
    {
        if (MY_RANK == i)
        {
            std::cout << " PROCESSOR " << std::setw(4) << MY_RANK+1 << " IS READY." << std::endl;
        }
        MPI_Barrier(MPI_COMM_WORLD);
    }
    */

    // This section can be chosen !!
    // mohan 2011-03-15
    if (MY_RANK != 0)
    {
        // std::cout.rdbuf(NULL);
        std::cout.setstate(std::ios::failbit); // qianrui modify 2020-10-14
    }
    // end test
#endif //__MPI
    return;
}

#ifdef __MPI
void Parallel_Global::finalize_mpi()
{
    MPI_Comm_free(&POOL_WORLD);
    if (INTER_POOL != MPI_COMM_NULL)
    {
        MPI_Comm_free(&INTER_POOL);
    }
    MPI_Comm_free(&STO_WORLD);
    MPI_Comm_free(&PARAPW_WORLD);
    MPI_Comm_free(&GRID_WORLD);
    MPI_Comm_free(&DIAG_WORLD);
    MPI_Finalize();
}
#endif

void Parallel_Global::init_pools(const int& NPROC,
                                 const int& MY_RANK,
                                 const int& NSTOGROUP,
                                 const int& KPAR,
                                 int& NPROC_IN_STOGROUP,
                                 int& RANK_IN_STOGROUP,
                                 int& MY_STOGROUP,
                                 int& NPROC_IN_POOL,
                                 int& RANK_IN_POOL,
                                 int& MY_POOL)
{
#ifdef __MPI
    //----------------------------------------------------------
    // CALL Function : divide_pools
    //----------------------------------------------------------
    Parallel_Global::divide_pools(NPROC,
                                  MY_RANK,
                                  NSTOGROUP,
                                  KPAR,
                                  NPROC_IN_STOGROUP,
                                  RANK_IN_STOGROUP,
                                  MY_STOGROUP,
                                  NPROC_IN_POOL,
                                  RANK_IN_POOL,
                                  MY_POOL);

    // for test
    // turn on when you want to check the index of pools.
    /*
        if (GlobalV::MY_RANK==0)
        {
            std::cout << "\n     " << std::setw(8) << "MY_RANK"
                 << std::setw(8) << "MY_POOL"
                 << std::setw(13) << "RANK_IN_POOL"
                 << std::setw(6) << "NPROC"
                 << std::setw(6) << "KPAR"
                 << std::setw(14) << "NPROC_IN_POOL" << std::endl;
        }
        for (int i=0; i<GlobalV::NPROC; i++)
        {
            if (GlobalV::MY_RANK == i)
            {
                std::cout << " I'm " << std::setw(8) << GlobalV::MY_RANK
                     << std::setw(8) << GlobalV::MY_POOL
                     << std::setw(13) << GlobalV::RANK_IN_POOL
                     << std::setw(6) << GlobalV::NPROC
                     << std::setw(6) << GlobalV::KPAR
                     << std::setw(14) << GlobalV::NPROC_IN_POOL << std::endl;
            }
            MPI_Barrier(MPI_COMM_WORLD);
        }

        if (GlobalV::MY_RANK != 0 )
        {
            std::cout.rdbuf(NULL);
        }
    */

    return;
#endif
}

#ifdef __MPI
void Parallel_Global::divide_pools(const int& NPROC,
                                   const int& MY_RANK,
                                   const int& NSTOGROUP,
                                   const int& KPAR,
                                   int& NPROC_IN_STOGROUP,
                                   int& RANK_IN_STOGROUP,
                                   int& MY_STOGROUP,
                                   int& NPROC_IN_POOL,
                                   int& RANK_IN_POOL,
                                   int& MY_POOL)
{
    // Divide the global communicator into stogroups.
    divide_mpi_groups(NPROC, NSTOGROUP, MY_RANK, NPROC_IN_STOGROUP, MY_STOGROUP, RANK_IN_STOGROUP, true);

    // (2) per process in each pool
    divide_mpi_groups(NPROC_IN_STOGROUP, KPAR, RANK_IN_STOGROUP, NPROC_IN_POOL, MY_POOL, RANK_IN_POOL);

    int key = 1;
    MPI_Comm_split(MPI_COMM_WORLD, MY_STOGROUP, key, &STO_WORLD);

    //========================================================
    // MPI_Comm_Split: Creates new communicators based on
    // colors(2nd parameter) and keys(3rd parameter)
    // Note: The color must be non-negative or MPI_UNDEFINED.
    //========================================================
    MPI_Comm_split(STO_WORLD, MY_POOL, key, &POOL_WORLD);

    if (NPROC_IN_STOGROUP % KPAR == 0)
    {
        MPI_Comm_split(STO_WORLD, RANK_IN_POOL, key, &INTER_POOL);
    }

    int color = MY_RANK % NPROC_IN_STOGROUP;
    MPI_Comm_split(MPI_COMM_WORLD, color, key, &PARAPW_WORLD);

    return;
}

void Parallel_Global::divide_mpi_groups(const int& procs,
                                        const int& num_groups,
                                        const int& rank,
                                        int& procs_in_group,
                                        int& my_group,
                                        int& rank_in_group,
                                        const bool even)
{
    if (num_groups == 0)
    {
        std::cout << "Error: Number of groups must be greater than 0." << std::endl;
        exit(1);
    }
    if (procs < num_groups)
    {
        std::cout << "Error: Number of processes (" << procs << ") must be greater than the number of groups ("
                  << num_groups << ")." << std::endl;
        exit(1);
    }
    // Calculate the distribution of processes among pools.
    procs_in_group = procs / num_groups;
    int extra_procs = procs % num_groups;

    if (even && extra_procs != 0)
    {
        std::cout << "Error: Number of processes (" << procs << ") must be evenly divisible by the number of groups ("
                  << num_groups << " in the even partition case)." << std::endl;
        exit(1);
    }

    int* nproc_group_ = new int[num_groups];

    for (int i = 0; i < num_groups; i++)
    {
        nproc_group_[i] = procs_in_group;
        if (i < extra_procs)
        {
            ++nproc_group_[i];
        }
    }

    int np_now = 0;
    for (int i = 0; i < num_groups; i++)
    {
        np_now += nproc_group_[i];
        if (rank < np_now)
        {
            my_group = i;
            procs_in_group = nproc_group_[i];
            rank_in_group = rank - (np_now - procs_in_group);
            break;
        }
    }

    delete[] nproc_group_;
}

#endif