void drawGrid(char ** const gridMarix, int height, int width, int startx, int starty){
    int limit = height * width;
    for (int i=0; i < height; i++){
        for (int j=0; j < width; j++)
            mvprintw(starty + i, startx + j,"%c ", gridMarix[i][j]);
    }
}

void initGrid(char ** const matrix, char * const data, int height, int width){
    for (int i=0; i < height * width; i++){
        data[i] = '.';
    }

    for (int i=0; i < height; i++){
        matrix[i] = data + width * i;
    }
}

void destroyGrid(char * const data){
    free(data);
}
