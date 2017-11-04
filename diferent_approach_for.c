/*  //pragma for
    for(int i=0; i<NUM_BODIES_PARAM; i++){
        fscanf(fptr, "%f", &data[i]);

    }
    
    // Initial conditions (total time and delta time between 2 interations)
    init.total_time=data[0];
    init.delta=data[1];
    int counter=2;
    for (int i=0; i<NUM_BODIES; i++, counter+=4){
            line_body[i].x=data[counter];
            line_body[i].y=data[counter+1];
            line_body[i].z=data[counter+2];
            line_body[i].m=data[counter+3];
    }
    for(int i=0; i<NUM_BODIES_PARAM; i++){
        printf("%.1f \n", data[i]);

    } */