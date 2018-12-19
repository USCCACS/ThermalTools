#define  Natomtype 2
int NFRAME;         //Total number of input frame
int NATOMS;         //Total number of atoms
int Ninitial;       //Total number of inital conditions 
int Corlength;      //Correlation length for each initial condition
int Ngap;           //Gap between two initial condition
int TFREQ;          //Timestep between two consecutive saved frame
int TNgapred;       //reduced gap between initial conditions
double  dT;         //timestep in ps 
double massW;       //mass of atom type 1
double massSe;      //mass of atom type 2
double kb=8.617*0.01;     // kb in meV

//DOS parameters
double omagaval=20;      // omagain THz   multiply this umber by 4.13 to convert to meV
int maxT=500;           //maxtime used in DOS calculation
//Specific heat parameters
double Temp_max=500;
double Temp_min=50;
double del_temp=50;

int Natom[Natomtype+1];
int timestep=0;
typedef struct atomvelocity{
	double *vx,*vy,*vz;
} velocity;

typedef struct velocity_series{
	velocity vel_t;
        struct velocity_series *vel_next_t;
        struct velocity_series *vel_prev_t;
} velocity_time;

/*-----------Link list functions---------*/
void list_init(velocity_time *vel_list){
	vel_list->vel_t.vx=NULL;
	vel_list->vel_t.vy=NULL;
	vel_list->vel_t.vz=NULL;
	vel_list->vel_next_t=vel_list;
	vel_list->vel_prev_t=vel_list;
}
void list_insert_tail(velocity_time *vel_list,double *vx,double *vy,double *vz){

	velocity_time *newvelocity_t,*tempval1;

	newvelocity_t=(struct velocity_series *)malloc(sizeof(velocity_time));

	newvelocity_t->vel_t.vx=vx;
	newvelocity_t->vel_t.vy=vy;
	newvelocity_t->vel_t.vz=vz;
        newvelocity_t->vel_next_t=NULL;
	newvelocity_t->vel_prev_t=NULL;

	tempval1=vel_list->vel_prev_t;
	newvelocity_t->vel_next_t=vel_list;
	vel_list->vel_prev_t=newvelocity_t;
        newvelocity_t->vel_prev_t=tempval1;
	tempval1->vel_next_t=newvelocity_t;

}

void list_head(velocity_time *vel_list,double **vx,double **vy,double **vz){
//	printf("vel  %lf %lf %lf\n",vel_list->vel_next_t->vel_t.vx[0],vel_list->vel_next_t->vel_t.vy[0],vel_list->vel_next_t->vel_t.vz[0]);
	*vx=(vel_list->vel_next_t->vel_t.vx);
	*vy=(vel_list->vel_next_t->vel_t.vy);
	*vz=(vel_list->vel_next_t->vel_t.vz);
        return;
}

void list_next_element(velocity_time *vel_list,double **vx,double **vy,double **vz){
	*vx=vel_list->vel_t.vx;
	*vy=vel_list->vel_t.vy;
	*vz=vel_list->vel_t.vz;
	return;
}
