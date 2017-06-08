ESTADO load() {
    FILE *fp;
    char line[MAX_BUFFER];

    ESTADO e;

    fp = fopen("/tmp/state.txt", "r");

    if(fp) {
        fgets(line,sizeof(line),fp);
        fclose(fp);
        e = str2estado(line);
    } else {
        e = inicializar();
    }
    return e;
}

ESTADO manageMov(int x) {
    ESTADO aux = load();
    switch(x) {
        case 1 :
        aux.jog.x -= 1;
        aux.jog.y -= 1;
    }
}