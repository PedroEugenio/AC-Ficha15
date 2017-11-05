#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#ifdef _OPENMP
    #include<omp.h>
#endif

#define NUM_BODIES_PARAM 4002
#define NUM_BODIES 1000

#define G 6.674E-2

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
    int i=0;
    struct initial_conditions init;

    float force;



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
 
    /* // Print initial values (total time and delta time)
    printf("%i %i\n", init.total_time, init.delta);
    // Print all values for our variables (x,y,z,m)
    //pragma for
    for(int i=0; i<NUM_BODIES; i++){
        printf("%.1f %.1f %.1f %.1f \n", line_body[i].x, line_body[i].y, line_body[i].z, line_body[i].m);
    } */

    #pragma omp parallel for
    for(int i=0; i<NUM_BODIES; i++){
        for(int j=0; j<NUM_BODIES; j++){
            force=(G*line_body[i].m*line_body[j].m)/sqr_module(module(diff(line_body[j],line_body[i])));
            temp=versor(line_body[j],line_body[i]);
            printf("f[%i][%i] = %f \n", i, j, force);
        }
}

    /* line_body[1].x=0;
    line_body[1].y=0;
    line_body[1].z=0;
    line_body[2].x=1;
    line_body[2].y=-5;
    line_body[2].z=1;

    line_body[3]=versor(line_body[2],line_body[1]);
    printf("%f \n", module(line_body[2]));
    printf("versor: %f %f %f", line_body[3].x, line_body[3].y, line_body[3].z); */
   
   return 0;
}