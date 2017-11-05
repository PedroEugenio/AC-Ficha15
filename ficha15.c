#include <stdlib.h>
#include <stdio.h>

#define NUM_BODIES_PARAM 4002
#define NUM_BODIES 1000

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

int main(){

    struct data line_body[NUM_BODIES];  // Array with data from each body
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
   
    printf("%.1f %.1f %.1f %.1f \n", line_body[0].x, line_body[0].y, line_body[0].z, line_body[0].m);
    // Verify if the document reached to an end
    while( !feof (fptr) ){ 
        // Saves the values from .dat file to the variables
        fscanf(fptr, "%f %f %f %f", &line_body[i].x, &line_body[i].y, &line_body[i].z, &line_body[i].m);
        //printf("%.1f %.1f %.1f %.1f \n", line_body[i].x, line_body[i].y, line_body[i].z, line_body[i].m);
        i++;
    }
    
    // Print initial values (total time and delta time)
    printf("%i %i\n", init.total_time, init.delta);

    // Print all values for our variables (x,y,z,m)
    //pragma for
    for(int i=0; i<NUM_BODIES; i++){
        printf("%.1f %.1f %.1f %.1f \n", line_body[i].x, line_body[i].y, line_body[i].z, line_body[i].m);
    }

    
   fclose(fptr);
   
   return 0;
}