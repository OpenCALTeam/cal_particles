#include <boundary.h>
#include <ep_collision.h>
#include <ep_movili.h>
#include <ep_movili_cazzu.h>
#include <ep_physics.h>
#include <ep_utils.h>
#include <init.h>
#include <model.h>
#include <utils_io.h>
#include <sim_stop.h>
#include <stdlib.h>
#include <unistd.h>

struct CALModel3D* u_modellu;
struct CALCLModel3D * device_CA;
struct CALRun3D* a_simulazioni;
struct Substates Q;
CALint initial_nummber_of_particles;
CALreal elapsed_time;

void updateF(struct CALModel3D* ca)
{
  for (int slot = 0; slot < MAX_NUMBER_OF_PARTICLES_PER_CELL; slot++)
    {
      calUpdateSubstate3Dr(ca,Q.Fx[slot]);
      calUpdateSubstate3Dr(ca,Q.Fy[slot]);
      calUpdateSubstate3Dr(ca,Q.Fz[slot]);
    }
}

void updateP(struct CALModel3D* ca)
{
  for (int slot = 0; slot < MAX_NUMBER_OF_PARTICLES_PER_CELL; slot++)
    {
      calUpdateSubstate3Dr(ca,Q.px[slot]);
      calUpdateSubstate3Dr(ca,Q.py[slot]);
      calUpdateSubstate3Dr(ca,Q.pz[slot]);
    }
}

void updateV(struct CALModel3D* ca)
{
  for (int slot = 0; slot < MAX_NUMBER_OF_PARTICLES_PER_CELL; slot++)
    {
      calUpdateSubstate3Dr(ca,Q.vx[slot]);
      calUpdateSubstate3Dr(ca,Q.vy[slot]);
      calUpdateSubstate3Dr(ca,Q.vz[slot]);
    }
}

void transizioniGlobali(struct CALModel3D* modello)
{
  calApplyElementaryProcess3D(modello, resetF);
  updateF(modello);

  calApplyElementaryProcess3D(modello,inner_collision);
  calApplyElementaryProcess3D(modello,outer_collision);
  updateF(modello);

  calApplyElementaryProcess3D(modello,movili);
  updateP(modello);
  updateV(modello);


  calApplyElementaryProcess3D(modello,moviliCazzu);
  calUpdate3D(modello);

  elapsed_time += DELTA_T;

#ifdef VERBOSE
  printSummary(modello);
#endif

  CALint S = INTEGRITY_CHECK_STEPS;
  if (a_simulazioni->step % S == 0)
    {
      CALint missing_particle = findMissingParticle(modello);
      if (missing_particle)
        {
          printf("ERROR: missing particle with ID %d\n", missing_particle);
          exit(EXIT_FAILURE);
        }
    }
}

void __calclGetDirFiles(char ** paths, int pathsNum, char *** files_names, int * num_files) {

  DIR *dir;
  struct dirent *ent;

//  char cwd[1024];
//  if (getcwd(cwd, sizeof(cwd)) != NULL)
//    {
//    fprintf(stdout, "Current working dir: %s\n", cwd);
//    fflush(stdout);
//    }
//  else
//    perror("getcwd() error");

//  strcat (cwd,"/");
//  strcat (cwd,paths[0]);
//  fprintf(stdout, "Current working dir: %s\n", cwd);
//  fflush(stdout);


  (*num_files) = 0;
  int i;
  for (i = 0; i < pathsNum; i++) {
      if ((dir = opendir(paths[i])) != NULL) {
          while ((ent = readdir(dir)) != NULL) {
              if (ent->d_name[0] != '.')
                (*num_files)++;
            }
          closedir(dir);

        } else {
          perror("could not open directory\n");
          exit(1);
        }
    }


  (*files_names) = (char**) malloc(sizeof(char*) * (*num_files));
  int count = 0;
  for (i = 0; i < pathsNum; i++) {
      if ((dir = opendir(paths[i])) != NULL) {
          while ((ent = readdir(dir)) != NULL) {
              if (ent->d_name[0] != '.') {
                  (*files_names)[count] = (char*) malloc(1 + sizeof(char) * (strlen(paths[i]) + strlen(ent->d_name)));
                  strcpy((*files_names)[count], paths[i]);
                  strcat((*files_names)[count], ent->d_name);
                  count++;
                }
            }
          closedir(dir);
        } else {
          perror("could not open directory\n");
          exit(1);
        }

    }


}

CALCLprogram __calclLoadProgram3D(CALCLcontext context, CALCLdevice device, char* path_user_kernel, char* path_user_include) {

  char kernel_source_directory[2048];
  strcpy(kernel_source_directory,KERNEL_SOURCE_DIR);
  char kernel_include_directory[2048];
  strcpy(kernel_include_directory,KERNEL_INCLUDE_DIR);

  char* u = " -cl-fp32-correctly-rounded-divide-sqrt -cl-mad-enable -cl-no-signed-zeros -cl-unsafe-math-optimizations -cl-finite-math-only -cl-fast-relaxed-math -cl-uniform-work-group-size "; //" -cl-denorms-are-zero -cl-finite-math-only -cl-fp32-correctly-rounded-divide-sqrt ";
  char* pathOpenCALCL= getenv("OPENCALCL_PATH");
  if (pathOpenCALCL == NULL) {
      perror("please configure environment variable OPENCALCL_PATH\n");
      printf("please configure environment variable OPENCALCL_PATH\n");
      fflush(stdout);
      exit(1);
    }
  char* tmp;
  if (path_user_include == NULL) {
      tmp = (char*) malloc(sizeof(char) * (strlen(pathOpenCALCL) + strlen(kernel_include_directory) + strlen(" -I ") + strlen(u) + 1));
      strcpy(tmp, " -I ");
    } else {
      tmp = (char*) malloc(sizeof(char) * (strlen(path_user_include) + strlen(pathOpenCALCL) + strlen(kernel_include_directory) + strlen(" -I ") * 2 + strlen(u) + 1));
      strcpy(tmp, " -I ");
      strcat(tmp, path_user_include);
      strcat(tmp, " -I ");
    }
  strcat(tmp, pathOpenCALCL);
  strcat(tmp, kernel_include_directory);
  strcat(tmp, u);

  int num_files;
  char** filesNames;
  char** paths = (char**) malloc(sizeof(char*) * 2);

  char* tmp2 = (char*) malloc(sizeof(char) * (1 + strlen(pathOpenCALCL) + strlen(kernel_source_directory)));
  strcpy(tmp2,pathOpenCALCL );
  strcat(tmp2,kernel_source_directory );

  paths[0] = path_user_kernel;
  paths[1] = tmp2;

  __calclGetDirFiles(paths, 2, &filesNames, &num_files);

  CALCLprogram program = calclGetProgramFromFiles(filesNames, num_files, tmp, context, &device, 1);
  int i;
  for (i = 0; i < num_files; i++) {
      free(filesNames[i]);
    }
  free(filesNames);
  free(tmp);
  return program;
}

void partilu()
{
  //setenv("CUDA_CACHE_DISABLE", "1", 1);

  // Select a compliant device
  struct CALCLDeviceManager * calcl_device_manager = calclCreateManager();
  calclPrintPlatformsAndDevices(calcl_device_manager);
  CALCLdevice device = calclGetDevice(calcl_device_manager, PLATFORM_NUM, DEVICE_NUM);
  CALCLcontext context = calclCreateContext(&device);

  // Load kernels and return a compiled program
  printf("KERNEL_SRC = %s\n", KERNEL_SRC);
  printf("KERNEL_INC = %s\n", KERNEL_INC);
  CALCLprogram program = __calclLoadProgram3D(context, device, KERNEL_SRC, KERNEL_INC);

  u_modellu = calCADef3D(X_CELLS,Y_CELLS,Z_CELLS,CAL_MOORE_NEIGHBORHOOD_3D,CAL_SPACE_TOROIDAL,CAL_NO_OPT);

  Q.Fx = (struct CALSubstate3Dr**)malloc(sizeof(struct CALSubstate3Dr*)*MAX_NUMBER_OF_PARTICLES_PER_CELL);
  Q.Fy = (struct CALSubstate3Dr**)malloc(sizeof(struct CALSubstate3Dr*)*MAX_NUMBER_OF_PARTICLES_PER_CELL);
  Q.Fz = (struct CALSubstate3Dr**)malloc(sizeof(struct CALSubstate3Dr*)*MAX_NUMBER_OF_PARTICLES_PER_CELL);
  Q.px = (struct CALSubstate3Dr**)malloc(sizeof(struct CALSubstate3Dr*)*MAX_NUMBER_OF_PARTICLES_PER_CELL);
  Q.py = (struct CALSubstate3Dr**)malloc(sizeof(struct CALSubstate3Dr*)*MAX_NUMBER_OF_PARTICLES_PER_CELL);
  Q.pz = (struct CALSubstate3Dr**)malloc(sizeof(struct CALSubstate3Dr*)*MAX_NUMBER_OF_PARTICLES_PER_CELL);
  Q.vx = (struct CALSubstate3Dr**)malloc(sizeof(struct CALSubstate3Dr*)*MAX_NUMBER_OF_PARTICLES_PER_CELL);
  Q.vy = (struct CALSubstate3Dr**)malloc(sizeof(struct CALSubstate3Dr*)*MAX_NUMBER_OF_PARTICLES_PER_CELL);
  Q.vz = (struct CALSubstate3Dr**)malloc(sizeof(struct CALSubstate3Dr*)*MAX_NUMBER_OF_PARTICLES_PER_CELL);
  Q.ID = (struct CALSubstate3Di**)malloc(sizeof(struct CALSubstate3Di*)*MAX_NUMBER_OF_PARTICLES_PER_CELL);

  for(int slot=0;slot<MAX_NUMBER_OF_PARTICLES_PER_CELL;slot++)
    {
      Q.Fx[slot] = calAddSubstate3Dr(u_modellu);
      Q.Fy[slot] = calAddSubstate3Dr(u_modellu);
      Q.Fz[slot] = calAddSubstate3Dr(u_modellu);
      Q.px[slot] = calAddSubstate3Dr(u_modellu);
      Q.py[slot] = calAddSubstate3Dr(u_modellu);
      Q.pz[slot] = calAddSubstate3Dr(u_modellu);
      Q.vx[slot] = calAddSubstate3Dr(u_modellu);
      Q.vy[slot] = calAddSubstate3Dr(u_modellu);
      Q.vz[slot] = calAddSubstate3Dr(u_modellu);
      Q.ID[slot] = calAddSubstate3Di(u_modellu);

      calInitSubstate3Dr(u_modellu,Q.Fx[slot],0.0);
      calInitSubstate3Dr(u_modellu,Q.Fy[slot],0.0);
      calInitSubstate3Dr(u_modellu,Q.Fz[slot],0.0);
      calInitSubstate3Dr(u_modellu,Q.px[slot],0.0);
      calInitSubstate3Dr(u_modellu,Q.py[slot],0.0);
      calInitSubstate3Dr(u_modellu,Q.pz[slot],0.0);
      calInitSubstate3Dr(u_modellu,Q.vx[slot],0.0);
      calInitSubstate3Dr(u_modellu,Q.vy[slot],0.0);
      calInitSubstate3Dr(u_modellu,Q.vz[slot],0.0);
      calInitSubstate3Di(u_modellu,Q.ID[slot],NULL_ID);
    }



  // Boundary
  boundaryCellsSerial(u_modellu);

  // Initial conditions
  initial_nummber_of_particles = 0;
  elapsed_time = 0.0;
  mmiscali_nta_cella_seriale(u_modellu);
  cancella_particelle_in_urto(u_modellu);

  // Define a device-side CA
  device_CA = calclCADef3D(u_modellu, context, program, device);

  CALCLkernel resetF_kernel      = calclGetKernelFromProgram(&program, RESETF);
  CALCLkernel collsion_kernel    = calclGetKernelFromProgram(&program, COLLISION);
  CALCLkernel movili_kernel      = calclGetKernelFromProgram(&program, MOVILI);
  CALCLkernel moviliCazzu_kernel = calclGetKernelFromProgram(&program, MOVILICAZZU);


  // Add transition function's elementary process
  calclAddElementaryProcess3D(device_CA, &resetF_kernel);
  calclAddElementaryProcess3D(device_CA, &collsion_kernel);
  calclAddElementaryProcess3D(device_CA, &movili_kernel);
  calclAddElementaryProcess3D(device_CA, &moviliCazzu_kernel);

  // Simulation
  //  a_simulazioni = calRunDef3D(u_modellu,0,CAL_RUN_LOOP,CAL_UPDATE_IMPLICIT);
  //  calRunAddGlobalTransitionFunc3D(a_simulazioni, transizioniGlobali);
  //  calRunAddStopConditionFunc3D(a_simulazioni, caminalu);



#ifdef VERBOSE
  printf("The 3D particles computational model\n");
#ifdef OMP
  printf("OpenMP parallel execution enabled!\n");
#endif
  printf("A system of %d particles will be simulated for %f s, subdivided in %d steps, each one corresponding to %f s\n", initial_nummber_of_particles, TOTAL_SIMULATION_TIME, STEPS, DELTA_T);
#endif
}
