void save(ESTADO e) {

    FILE *fp; 
    char line[MAX_BUFFER];
    line = estado2str(e)

    fp = fopen("./score/state.txt", "w");

    if(fp) {
        fputs(line, fp);
    }
}

ESTADO load() {
    FILE *fp;
    char line[MAX_BUFFER];

    ESTADO e;

    fp = fopen("./score/state.txt", "r");

    if(fp) {
        fgets(line,sizeof(line),fp);
        fclose(fp);
        e = str2estado(line);
    } else {
        e = inicializar();
        save(e);
    }
    return e;
}

ESTADO manageMov(ESTADO e, int x) {


    switch(x) {
        case 1 :
            e.jog.x -= 1;
            e.jog.y -= 1;
            break;
        case 2 :
            e.jog.y -= 1;
            break;
        case 3 :
            e.jog.x += 1;
            e.jog.y -= 1;
            break;
        case 4 :
            e.jog.x -= 1;
            break;
        case 5 :
            break;
        case 6 :
            e.jog.x += 1;
            break;
        case 7 :
            e.jog.x -= 1;
            e.jog.y += 1;
            break;
        case 8 :
            e.jog.y += 1;
            break;
        case 9 :
            e.jog.x += 1;
            e.jog.y += 1;
            break;
        default:
            break;
    }

    return e;
}

char* load() {
    FILE *fp;
    char line[MAX_BUFFER];

    fp = fopen("./score/state.txt", "r");

    if(fp) {
        fgets(line,sizeof(line),fp);
        fclose(fp);
    }
    return line;
}


int teste() {
    char *teste;

    teste = getenv("QUERY_STRING");

    if(strlen(teste) > 0) {
        sscanf(teste, "x=%d&y=%d", &x, &y);
    }
   
}