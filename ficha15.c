#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#ifdef _OPENMP
    #include<omp.h>
#endif

#define NUM_BODIES_PARAM 4002
#define NUM_BODIES 1000
#define G 6.674E-11

struct data{
    float x;
    float y;
    float z;
    float m;
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

struct initial_conditions{
    int total_time;
    int delta;
};


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

    struct data line_body[NUM_BODIES];  // Array with data from each body
    struct data temp;

    struct force f[NUM_BODIES][NUM_BODIES];
    float force_f;
    struct force fi[NUM_BODIES];

    struct Matrix *mat;

    int i=0;
    struct initial_conditions init;

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

    // Print initial values (total time and delta time)
    printf("%i %i\n", init.total_time, init.delta);
    // Print all values for our variables (x,y,z,m)
    for(int i=0; i<NUM_BODIES; i++){
        printf("%.1f %.1f %.1f %.1f \n", line_body[i].x, line_body[i].y, line_body[i].z, line_body[i].m);
    }

    /* // The total number of elements in the matrix is
    // the number of columns times the number of rows
    mat->values = malloc(NUM_BODIES * NUM_BODIES * sizeof(struct force));
    if (mat->values == NULL) {
        fprintf(stderr, "Out-of-memory");
    }
    mat->width  = NUM_BODIES;
    mat->height = NUM_BODIES; */

    /* //#pragma omp parallel for
    for(int i=0; i<NUM_BODIES; i++){
        for(int j=0; j<NUM_BODIES; j++){
            force_f=(G*line_body[i].m*line_body[j].m)/sqr_module(module(diff(line_body[j],line_body[i])));
            temp=versor(line_body[j],line_body[i]);
            mat->values[(i * mat->width) + j].x=force_f*temp.x;
            mat->values[(i * mat->width) + j].y=force_f*temp.y;
            mat->values[(i * mat->width) + j].z=force_f*temp.z;
        }
    } */
/* 
    //#pragma omp parallel for
    for(int i=0; i<NUM_BODIES; i++){
        for(int j=0; j<NUM_BODIES; j++){
            force_f=(G*line_body[i].m*line_body[j].m)/sqr_module(module(diff(line_body[j],line_body[i])));
            temp=versor(line_body[j],line_body[i]);
            f[i][j].x=force_f*temp.x;
            f[i][j].y=force_f*temp.y;
            f[i][j].z=force_f*temp.z;
        }
    } */


    line_body[1].x=0;
    line_body[1].y=0;
    line_body[1].z=0;
    line_body[2].x=1;
    line_body[2].y=-5;
    line_body[2].z=1;

    line_body[3]=versor(line_body[2],line_body[1]);
    printf("%f \n", module(line_body[2]));
    printf("versor: %f %f %f", line_body[3].x, line_body[3].y, line_body[3].z);
   
   return 0;
}