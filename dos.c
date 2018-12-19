#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "dos.h"

static void readinput_parameter(char *inputfilename){
       FILE *fp;
       char buf[1024];
       char field1[100],field2[100];
       int counter=0;
       fp=fopen(inputfilename,"r");
       if(fp==NULL){
	       fprintf(stderr,"input  parameter file: input.txt doesnot exists\n");
	       exit(1);
       }else{
	       while(fgets(buf,sizeof(buf),fp) !=NULL){
		       if(buf[strlen(buf)-1] !='\n'){
			     fprintf(stderr, "Length of the input string is too large\n");
			     exit(1);
		       }else{
			    counter++;
			    sscanf(buf,"%s %s",field1,field2);
			    if (counter==1)       Ninitial=atoi(field2);
			    else if (counter==2)  Corlength=atoi(field2);
			    else if (counter==3)  Ngap=atoi(field2);
			    else if (counter==4)  TFREQ=atoi(field2);
			    else if (counter==5)  dT=atof(field2);
			    else if (counter==6)  massW=atof(field2);
			    else if (counter==7)  massSe=atof(field2);
		       }
	       }
       }
       TNgapred=(int)Ngap/TFREQ;
       printf("reduced gap: %d\n",TNgapred);
       return;
}

static void readinput_data(char *filename,velocity_time **retval){
	FILE *fp;
	int count=0,step=0;
	int ii=0;
	char buf[1024];
	int igd,prev_id,itype;
	double rx,ry,rz;
	double *vx0,*vy0,*vz0,*vx1,*vy1,*vz1,*vx2,*vy2,*vz2;
	int i_counter[Natomtype+1];
	velocity_time *atomvelocities[Natomtype+1];

	fp=fopen(filename,"r");
	if(fp==NULL){
		fprintf(stderr,"Lammps dump file: %s doesnot exists.\n",filename);
		exit(1);
	}
	/*---initilizes the link list for velocities of atoms of different types and one for all atoms*/
	for (ii=0;ii < Natomtype+1;ii++){
		atomvelocities[ii]=(velocity_time *)malloc(sizeof(velocity_time));
		list_init(atomvelocities[ii]);
	}
        /*-----read input velocities for 0th step-------*/
	while (fgets(buf,sizeof(buf),fp) != NULL){
		if(buf[strlen(buf)-1] !='\n'){
			fprintf(stderr, "Length of the input string is too large\n");
                        exit(1);
		}
		count++;
		if (count <=3)  continue;
		else if (count ==4)  {
			sscanf(buf,"%d",&Natom[0]);
			vx0=(double *)malloc(Natom[0]*sizeof(double));
			vy0=(double *)malloc(Natom[0]*sizeof(double));
			vz0=(double *)malloc(Natom[0]*sizeof(double));
                        vx1=(double *)malloc(Natom[0]*sizeof(double));
			vy1=(double *)malloc(Natom[0]*sizeof(double));
			vz1=(double *)malloc(Natom[0]*sizeof(double));
			vx2=(double *)malloc(Natom[0]*sizeof(double));
			vy2=(double *)malloc(Natom[0]*sizeof(double));
			vz2=(double *)malloc(Natom[0]*sizeof(double));
			i_counter[0]=-1;
			i_counter[1]=-1;
			i_counter[2]=-1;
		}
		else if (count >=5 && count <=9) continue;
		else if (count>=10 && count <=9+Natom[0]){
			if(count==10) sscanf(buf,"%d",&prev_id);
			if (count >= 10){
				i_counter[0]++;
				sscanf(buf,"%d %d %lf %lf %lf %lf %lf %lf",&igd,&itype,&rx,&ry,&rz,&vx0[i_counter[0]],&vy0[i_counter[0]],&vz0[i_counter[0]]);
				if (igd < prev_id){
					fprintf(stderr,"Data is not sorted\n");
					exit(1);
				}
				i_counter[itype]++;
				if (itype==1) {
					vx1[i_counter[itype]]=vx0[i_counter[0]];
					vy1[i_counter[itype]]=vy0[i_counter[0]];
					vz1[i_counter[itype]]=vz0[i_counter[0]];
				}else if (itype==2){
					vx2[i_counter[itype]]=vx0[i_counter[0]];
					vy2[i_counter[itype]]=vy0[i_counter[0]];
					vz2[i_counter[itype]]=vz0[i_counter[0]];
				}
			}
			if(count == 9+Natom[0]){
				count=0;
				step++;
				//printf("step %d : \n",step);
				list_insert_tail(atomvelocities[0],vx0,vy0,vz0);
				list_insert_tail(atomvelocities[1],vx1,vy1,vz1);
				list_insert_tail(atomvelocities[2],vx2,vy2,vz2);
			}
		}
	}
	timestep=step;
	for (ii=0;ii < Natomtype+1;ii++){
		Natom[ii]=i_counter[ii]+1;
		retval[ii]=atomvelocities[ii];
	}
	return;
}

static double** init_velocity_corr_array(int n_rows,int n_column){
	double **vel;
	int ii=0,jj=0;
	
	vel=(double **)malloc(n_rows*sizeof(double *));
       
	for(ii=0;ii<n_rows;ii++){
		vel[ii]=(double *)malloc(n_column*sizeof(double));
		for(jj=0;jj<n_column;jj++){
			vel[ii][jj]=0;
		}
	}
	return vel;
}

static void velocity_corr(int Natom,int NFRAME,velocity_time *atomvelocities,double **V_f,double **V_fx,double **V_fy,double **V_fz,double **t_val){

	velocity_time *vel_t=atomvelocities->vel_next_t;
	int count=0,nn,kk,mm,ii;
	int r_Corlength=(int)(Corlength/TFREQ);
	double **velocoor,**velocoorx,**velocoory,**velocoorz;
	double **Vint_x0,**Vint_y0,**Vint_z0;
        double *vx,*vy,*vz;
	double *Vcoorfinal,*Vcoorfinalx,*Vcoorfinaly,*Vcoorfinalz,*timeval;
	double vel_auto,vel_autox,vel_autoy,vel_autoz;
        double temp1,temp1_x,temp1_y,temp1_z;

        //initialize the parameters for the velocity autocorrelation 	
	velocoor=init_velocity_corr_array(Ninitial,r_Corlength);
	velocoorx=init_velocity_corr_array(Ninitial,r_Corlength);
	velocoory=init_velocity_corr_array(Ninitial,r_Corlength);
	velocoorz=init_velocity_corr_array(Ninitial,r_Corlength);
	Vint_x0=(double **)malloc(Ninitial*sizeof(double *));
	Vint_y0=(double **)malloc(Ninitial*sizeof(double *));
	Vint_z0=(double **)malloc(Ninitial*sizeof(double *));
	Vcoorfinal=(double *)malloc(r_Corlength*sizeof(double));
	Vcoorfinalx=(double *)malloc(r_Corlength*sizeof(double));
	Vcoorfinaly=(double *)malloc(r_Corlength*sizeof(double));
	Vcoorfinalz=(double *)malloc(r_Corlength*sizeof(double));
        timeval=(double *)malloc(r_Corlength*sizeof(double));

        //printf("inside velocity_corr %d\n",NFRAME);

	for(nn=0;nn<NFRAME;nn++){
		list_next_element(vel_t,&vx,&vy,&vz);
		// Add a new initial condition
		if ( (nn==0 && count < Ninitial) ||( (nn%TNgapred)==0 && count < Ninitial) ){
			fprintf(stdout,"New Initial condition added: %d at time step: %f \n",count,nn*TNgapred*dT*TFREQ);
			Vint_x0[count]=vx;
			Vint_y0[count]=vy;
			Vint_z0[count]=vz;
			count++;
		//	printf("velocity %lf %lf %lf\n",vx[0],vy[0],vz[0]);
		}
		for(kk=0;kk<Ninitial;kk++){
			if((nn-kk*TNgapred) >=0 && (nn-kk*TNgapred) < r_Corlength){
				mm=nn-kk*TNgapred;
				vel_auto=0;
				vel_autox=0;
				vel_autoy=0;
				vel_autoz=0;
				for(ii=0;ii<Natom;ii++){
					vel_auto  += vx[ii]*Vint_x0[kk][ii]+vy[ii]*Vint_y0[kk][ii]+vz[ii]*Vint_z0[kk][ii];
					vel_autox += vx[ii]*Vint_x0[kk][ii];
					vel_autoy += vy[ii]*Vint_y0[kk][ii];
 					vel_autoz += vz[ii]*Vint_z0[kk][ii];
				}
				velocoor[kk][mm]=vel_auto;
				velocoorx[kk][mm]=vel_autox;
				velocoory[kk][mm]=vel_autoy;
				velocoorz[kk][mm]=vel_autoz;
			}
		}
		vel_t=vel_t->vel_next_t;
	}
	//Normalize  the velocity auto-correlation value
	 for(nn=0;nn<Ninitial;nn++){
		 vel_auto=0;
		 for(ii=0;ii<Natom;ii++){
			 vel_auto  += Vint_x0[nn][ii]*Vint_x0[nn][ii]+Vint_y0[nn][ii]*Vint_y0[nn][ii]+Vint_z0[nn][ii]*Vint_z0[nn][ii];
		 }
		 for(kk=0;kk<r_Corlength;kk++){
			velocoor[nn][kk] /= vel_auto;
			velocoorx[nn][kk] /= vel_auto;
			velocoory[nn][kk] /= vel_auto;
			velocoorz[nn][kk] /= vel_auto;
		 }
	 }
	 //Final velocity auto-correlation value by summing values from all initial condition
	 for(kk=0;kk<r_Corlength;kk++){
		 temp1=0;
		 temp1_x=0;
		 temp1_y=0;
		 temp1_z=0;
		 for(nn=0;nn<Ninitial;nn++){
			 temp1 += velocoor[nn][kk];
			 temp1_x += velocoorx[nn][kk];
			 temp1_y += velocoory[nn][kk];
			 temp1_z += velocoorz[nn][kk];
		 }
		 Vcoorfinal[kk]=temp1/Ninitial;
		 Vcoorfinalx[kk]=temp1_x/Ninitial;
		 Vcoorfinaly[kk]=temp1_y/Ninitial;
		 Vcoorfinalz[kk]=temp1_z/Ninitial;
		 timeval[kk]=kk*TFREQ*dT;
	 }
	 *V_f=Vcoorfinal;
	 *V_fx=Vcoorfinalx;
	 *V_fy=Vcoorfinaly;
	 *V_fz=Vcoorfinalz;
	 *t_val=timeval;
	return;
}

static void Densityofstate(double *input_vcorr,int nlocal_atoms,double **freq_ret,double **dosval_ret){

       double domaga=omagaval/(double)maxT;
       double Tdelta=dT*Corlength*0.5;
       int r_Corlength=(int)(Corlength/TFREQ);
       double *freq,*dosval;
       double dw,dstate,dtw;
       int ii,jj;
 
       freq=(double *)malloc(maxT*sizeof(double)); 
       dosval=(double *)malloc(maxT*sizeof(double));     

       for(ii=0;ii<maxT;ii++){
	     dw=ii*domaga;
	     dstate=0;
	     for(jj=0;jj<r_Corlength;jj++){
		      dtw=(double)(dT*jj*TFREQ);
		      dstate+=dT*TFREQ*input_vcorr[jj]*cos(2*3.14*dw*dtw)*exp(-1*pow((dtw/Tdelta),2));
	     }
	     freq[ii]=4.13*dw;
	     dosval[ii]=6.0*nlocal_atoms*dstate*0.31847;
       }
       *freq_ret=freq;
       *dosval_ret=dosval;
       return;
}
static void check_error(){
	int ii,sum=0;
        int Tlengthreg=0;

	for(ii=1;ii<Natomtype+1;ii++)
		sum+=Natom[ii];
        if(sum !=Natom[0]){
		fprintf(stdout,"Total number of atoms doesnot match\n");
		exit(1);
	}
	Tlengthreg=Ngap*Ninitial+Corlength;
	fprintf(stdout,"Total number of input frame     : %d \n",timestep);
	fprintf(stdout,"Total number of frame needed    : %d \n",Tlengthreg);
	fprintf(stdout,"Total number of frame available : %d \n",timestep*TFREQ);
	if(Tlengthreg > timestep*TFREQ ) {
		fprintf(stderr,"Frame required %d is greater then available frames %d\n",Tlengthreg,timestep*TFREQ);
		exit(1);
	}
}

static void specificheat(double *freq,double *dos_total,double **cv_val,double **temperatures){
	int t_range=(int)(Temp_max-Temp_min)/del_temp;
	double *temp_cv,*temp_temperature;
        double delomaga;
	double bottomval,topval,uu,utop,ubot;
        int tt,ii;

	delomaga=freq[1]-freq[0];
	temp_cv=(double  *)malloc(t_range*sizeof(double));
	temp_temperature=(double  *)malloc(t_range*sizeof(double));
        
	for(tt=0;tt<t_range;tt++){
		temp_temperature[tt]=(tt+1)*del_temp;
		topval=0;
		bottomval=0;
		for(ii=1;ii<maxT;ii++){
			 uu=freq[ii]/(kb*temp_temperature[tt]);
			 utop=uu*uu*exp(uu);
			 ubot=(exp(uu)-1)*(exp(uu)-1);
			 topval += (utop/ubot)*dos_total[ii]*delomaga;
			 bottomval += dos_total[ii] * delomaga;
		}
		temp_cv[tt]=topval/bottomval;
	}
        *cv_val=temp_cv;
	*temperatures=temp_temperature;
	return;
}

static void writeoutput(double *frequency,double *dosval,int N_length,char *filename){

	FILE *fp=fopen(filename,"w");
        int ii;
	for(ii=0;ii<N_length;ii++){
		fprintf(fp,"%lf %lf\n",frequency[ii],dosval[ii]);
	}
	return;
}
int main(int argc,char *argv[]){
	char filename[100];
        char inputfilename[100];
	double *vx,*vy,*vz;
	velocity_time *atomvelocities[Natomtype+1];
	double *Vcoorfinal,*Vcoorfinalx,*Vcoorfinaly,*Vcoorfinalz,*timeval;
        double *freq[Natomtype+1],*dosval[Natomtype+1]; 
	double *dos_total;
	double *cv_val,*temperatures;

	int ii;
	strcpy(inputfilename, argv[2]);
        readinput_parameter(inputfilename);
	fprintf(stdout,"input paramters: %d %d %d %d %f %f %f\n",Ninitial,Corlength,Ngap,TFREQ,dT,massW,massSe);
	strcpy(filename,argv[1]);
	readinput_data(filename,atomvelocities);
        check_error();

	//computes velocity autocorrelation and partial density of states
        for(ii=1;ii<Natomtype+1;ii++){
		fprintf(stdout,"computing VOS and PDOS for atom type %d\n",ii);
		velocity_corr(Natom[ii],timestep,atomvelocities[ii],&Vcoorfinal,&Vcoorfinalx,&Vcoorfinaly,&Vcoorfinalz,&timeval);
                Densityofstate(Vcoorfinal,Natom[ii],&freq[ii],&dosval[ii]);
		if(ii==1) {
			writeoutput(timeval,Vcoorfinal,(int)(Corlength/TFREQ),"VOC_typeA.txt");
			writeoutput(freq[ii],dosval[ii],maxT,"PDOS_typeA.txt");
		}
		if(ii==2) {
		        writeoutput(timeval,Vcoorfinal,(int)(Corlength/TFREQ),"VOC_typeB.txt");
			writeoutput(freq[ii],dosval[ii],maxT,"PDOS_typeB.txt");
		}
	}
	//computes total density of states and specific heat
	fprintf(stdout,"computing Total DOS and Specific heat\n");
 	dos_total=(double  *)malloc(maxT*sizeof(double));
	for (ii=0;ii<maxT;ii++) {
		dos_total[ii]=dosval[1][ii]+dosval[2][ii];
	}
	writeoutput(freq[1],dos_total,maxT,"DOS_total.txt");
	specificheat(freq[1],dos_total,&cv_val,&temperatures);
        writeoutput(temperatures,cv_val,(int)(Temp_max-Temp_min)/del_temp,"Specific_heat.txt");
	return 0;
}

