#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#ifdef _OPENMP
    #include<omp.h>
#endif

#define NUM_BODIES_PARAM 4002
#define NUM_BODIES 1000

#define G 6.674E-6

struct data{
    float x;
    float y;
    float z;
    float m;
};

struct initial_conditions{
    int total_time;
    int delta;
};

typedef struct force{
    float x;
    float y;
    float z;
}FORCE;

struct Matrix{
    FORCE * values;
    int width;
    int height;
};

////////////////////////////////////////////////////////////////////////////
struct timespec timespecDiff(struct timespec a, struct timespec b)
{
	struct timespec result;
	if ((a.tv_nsec - b.tv_nsec) < 0)
	{
		result.tv_sec = a.tv_sec - b.tv_sec - 1;
		result.tv_nsec = 1E9 + a.tv_nsec - b.tv_nsec;
	}
	else
	{
		result.tv_sec = a.tv_sec - b.tv_sec;
		result.tv_nsec = a.tv_nsec - b.tv_nsec;
	}
	return result;
}

long double timespecInS(struct timespec a)
{
	return (long double)(a.tv_sec + a.tv_nsec / 1E9);
}
////////////////////////////////////////////////////////////////////////////

struct data diff(struct data s2, struct data s1){
    struct data temp;

    temp.x=s2.x-s1.x;
    temp.y=s2.y-s1.y;
    temp.z=s2.z-s1.z;

    return temp;
}

float module(struct data temp){
    return sqrt((temp.x * temp.x) + (temp.y * temp.y) + (temp.z * temp.z));
}

float sqr_module(float mod){
    return mod*mod;
}

struct data versor(struct data s2, struct data s1){
    struct data vec, temp;
    vec=diff(s2,s1);
    float denum = module(vec);
    temp.x=vec.x/denum;
    temp.y=vec.y/denum;
    temp.z=vec.z/denum;

    // To make a unit vector
    if(temp.x < 0){
        temp.x = -1.0;
    }
    else if(temp.x > 0){
        temp.x = 1.0;
    }
    else{
        temp.x = 0.0;
    }
    if(temp.y < 0){
        temp.y = -1.0;
    }
    else if(temp.y > 0){
        temp.y = 1.0;
    }
    else{
        temp.y = 0.0;
    }
    if(temp.z < 0){
        temp.z = -1.0;
    }
    else if(temp.z > 0){
        temp.z = 1.0;
    }
    else{
        temp.z = 0.0;
    }

    return temp;
}


int main(){

    struct timespec start, end;

    struct data line_body[NUM_BODIES];  // Array with data from each body
    struct data temp;
    int i=0;
    struct initial_conditions init;

    float force;

    struct force a[NUM_BODIES];
    struct force v[NUM_BODIES];
    struct force p[NUM_BODIES];

    struct force fi[NUM_BODIES];

    struct Matrix *mat;



    printf("Opening the file \n");
    FILE *fptr;
    fptr = fopen("particles.dat","r");  // Open the file

        if(fptr == NULL)
        {
            perror("fopen()"); 
            exit(1);             
        }
        // Saves the values from .dat file to the initial variables
        fscanf(fptr, "%i %i", &init.total_time, &init.delta);
    
        // Verify if the document reached to an end
        while( !feof (fptr) ){ 
            // Saves the values from .dat file to the variables
            fscanf(fptr, "%f %f %f %f", &line_body[i].x, &line_body[i].y, &line_body[i].z, &line_body[i].m);
            //printf("%.1f %.1f %.1f %.1f \n", line_body[i].x, line_body[i].y, line_body[i].z, line_body[i].m);
            i++;
        }
       
   fclose(fptr);
   printf("Closing the file \n");

    // Allocate memory for mat
    mat=(struct Matrix *)malloc(sizeof(struct Matrix));
    // The total number of elements in the matrix is
    // the number of columns times the number of rows
    mat->values =(FORCE *) malloc(NUM_BODIES * NUM_BODIES * sizeof(FORCE));
    if (mat->values == NULL) {
        fprintf(stderr, "Out-of-memory");
    }
    mat->width  = NUM_BODIES;
    mat->height = NUM_BODIES;

    clock_gettime(CLOCK_MONOTONIC, &start);
    #pragma omp parallel for
    for(int i=0; i<NUM_BODIES; i++){
        for(int j=0; j<NUM_BODIES; j++){
            force=(G*line_body[i].m*line_body[j].m)/sqr_module(module(diff(line_body[j],line_body[i])));
            temp=versor(line_body[j],line_body[i]);
            mat->values[(i * mat->width) + j].x=force*temp.x;
            mat->values[(i * mat->width) + j].y=force*temp.y;
            mat->values[(i * mat->width) + j].z=force*temp.z;
        }
    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    printf("Total time: %LF s \n", timespecInS(timespecDiff(end, start)));

    /* clock_gettime(CLOCK_MONOTONIC, &start);
    // Prints all the values for f[i][j]
    for(int i=0; i<NUM_BODIES; i++){
        for(int j=0; j<NUM_BODIES; j++){
            printf("f[%i][%i]: x: %.10f y: %.10f z: %.10f \n",i,j, mat->values[(i * mat->width) + j].x,mat->values[(i * mat->width) + j].y=force*temp.y,mat->values[(i * mat->width) + j].z=force*temp.z);
        }
    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    printf("Total time: %LF s \n", timespecInS(timespecDiff(end, start))); */

    // Calculation of fi force
    fi[0].x=0;
    fi[0].y=0;
    fi[0].z=0;
    #pragma omp parallel for
    for(int i=0; i<NUM_BODIES ; i++){
        for(int j=0; j<NUM_BODIES && j!=i; j++){
            fi[i].x+=mat->values[(i * mat->width) + j].x;
            fi[i].y+=mat->values[(i * mat->width) + j].y;
            fi[i].z+=mat->values[(i * mat->width) + j].z;
        }
    }

    // Calculation of acelaration
    a[0].x=0;
    a[0].y=0;
    a[0].z=0;
    #pragma omp parallel for
    for(int i=0; i<NUM_BODIES ; i++){
            a[i].x=fi[i].x/line_body[i].m;
            a[i].y=fi[i].y/line_body[i].m;
            a[i].z=fi[i].z/line_body[i].m;
    }
    /* for(int i=0; i<NUM_BODIES; i++){
        printf("a[%i]: x: %.10f y: %.10f z: %.10f \n",i,a[i].x, a[i].y, a[i].z);
    } */
    // Calculation of velocity
    v[0].x=0;
    v[0].y=0;
    v[0].z=0;
    for(int i=1; i<NUM_BODIES ; i++){
            v[i].x=v[i-1].x+a[i].x*init.delta;
            v[i].y=v[i-1].y+a[i].y*init.delta;
            v[i].z=v[i-1].y+a[i].z*init.delta;
    }

    /* for(int i=0; i<NUM_BODIES; i++){
            printf("v[%i]: x: %.10f y: %.10f z: %.10f \n",i,v[i].x, v[i].y, v[i].z);
    } */

    //New Positions
    p[0].x=0;
    p[0].y=0;
    p[0].z=0;
    for(int i=1; i<NUM_BODIES ; i++){
        p[i].x=p[i-1].x+v[i].x*init.delta;
        p[i].y=p[i-1].y+v[i].y*init.delta;
        p[i].z=p[i-1].y+v[i].z*init.delta;
    }

    /* for(int i=0; i<NUM_BODIES; i++){
        printf("p[%i]: x: %.10f y: %.10f z: %.10f \n",i,p[i].x, p[i].y, p[i].z);
    } */
    printf("End of program! \n");
	
	/* Export coord. of the new position to a file (newposition.dat)*/
	
	FILE *newposition;
	newposition = fopen ("newposition.dat", "w");
	
	for(int i=0; i<NUM_BODIES; i++){
       	fprintf(newposition,"%.10f %.10f %.10f \n\n",p[i].x, p[i].y, p[i].z);
		
    }
    
	fclose(newposition);
	
	
    free(mat->values);
    free(mat);
   return 0;
}
