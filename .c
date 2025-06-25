#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct nodo_Denuncias {
    struct Denuncias *Denuncias;
    struct nodo_Denuncias *sig;
};

struct Denuncias {
    int RUC;
    char Tipo_denunciante[30];
    char descripcion[256];
    int fecha;
    struct nodo_carpeta *carpeta;
};

struct nodo_carpeta {
    struct Carpeta *carpeta;
    struct nodo_carpeta *sig;
    struct nodo_carpeta *ant;
};

struct Carpeta {
    struct nodo_imputado *imputados;
    int RUC;
    char testigos[256];
    char victimas[256];
    char resolucion[100];
    char declaraciones[256];
    char pruebas[256];
};

struct nodo_imputado {
    struct nodo_imputado *izq;
    struct nodo_imputado *der;
    struct imputado *datos;
};

struct imputado {
    char rut[15];
    char nombre[30];
    char estadoProcesal[30];
    char medidasCautelares[50];
    char defensor[30];
};

struct fiscal {
    char nombre_fiscal[30];
    char diligencias[100];
};


struct causa {
    int RUC;
    int urgencia;
    char estado_causa[30];
    struct nodo_Denuncias *denuncia;
    struct nodo_carpeta *carpeta;
    struct nodo_imputado *imputado;
    struct fiscal *fiscal;
};

//FUNCIONES DENUNCIAS
struct nodo_Denuncias *lista_denuncias = NULL;
struct Denuncias *crearDenuncia(int RUC, const char *tipo_denunciante, const char *descripcion, int fecha) {
    struct Denuncias *nueva = malloc(sizeof(struct Denuncias));
    if(nueva == NULL)
        return NULL;
    nueva->RUC = RUC;
    strcpy(nueva->Tipo_denunciante, tipo_denunciante);
    strncpy(nueva->descripcion, descripcion, sizeof(nueva->descripcion) - 1);
    nueva->descripcion[sizeof(nueva->descripcion) - 1] = '\0';
    nueva->fecha = fecha;
    nueva->carpeta = NULL;
    return nueva;
}

void imprimir_nueva_Denuncia(int RUC, const char *Tipo_denunciante, const char *descripcion, int fecha) {
    printf("Denuncia RUC:%d tipo de denunciante:%s  fecha:%d\n", RUC,Tipo_denunciante , fecha);
    printf("Descripcion: %s\n",descripcion);
    printf("DENUNCIA INGRESADA CORRECTAMENTE\n");
}

void imprimir_error_Denuncia(int RUC){
    printf("\nERROR INGRESAR RUC:%d o en alguna de los datos vuelva a intentarlo\n",RUC);
}

void agregarDenuncia(struct nodo_Denuncias **lista, int RUC, const char *tipo_Denuncias, const char *descripcion, int fecha) {
    struct nodo_Denuncias *nuevoNodo = malloc(sizeof(struct nodo_Denuncias));
    struct Denuncias *nuevaDenuncia = malloc(sizeof(struct Denuncias));

    if (!nuevoNodo || !nuevaDenuncia) {
        imprimir_error_Denuncia(RUC);
        free(nuevoNodo);
        free(nuevaDenuncia);
        return;
    }

    nuevaDenuncia->RUC = RUC;
    strcpy(nuevaDenuncia->Tipo_denunciante, tipo_Denuncias);
    strncpy(nuevaDenuncia->descripcion, descripcion, sizeof(nuevaDenuncia->descripcion) - 1);
    nuevaDenuncia->fecha = fecha;
    nuevaDenuncia->carpeta = NULL;

    nuevoNodo->Denuncias = nuevaDenuncia;
    nuevoNodo->sig = NULL;

    if (*lista == NULL) {
        *lista = nuevoNodo;
    } else {
        struct nodo_Denuncias *actual = *lista;
        while (actual->sig)
            actual = actual->sig;
        actual->sig = nuevoNodo;
    }

    imprimir_nueva_Denuncia(RUC,tipo_Denuncias, descripcion, fecha);
}

void imprimir_eliminar_correcta(int RUC){
    printf("\nLa eliminacion se hizo correcta en la denuncia con RUC:%d\n",RUC);
}

void imprimir_lista_vacia(int RUC){
    printf("\nNo existe esta denunica o el RUC:%d no fue encotrada\n",RUC);
}
void eliminarDenuncia(struct nodo_Denuncias **lista, int RUC){
    struct nodo_Denuncias *actual = *lista;
    struct nodo_Denuncias *anterior = NULL;

    if(*lista == NULL){
        imprimir_lista_vacia(RUC);
        return;
    }

    while(actual != NULL && actual->Denuncias->RUC != RUC)
    {
        anterior = actual;
        actual = actual->sig;
    }

    if(actual == NULL) {
        imprimir_lista_vacia(RUC);
        return;
    }

    if(anterior == NULL) {
        *lista = actual->sig;
    } else {
        anterior->sig = actual->sig;
    }

    free(actual->Denuncias);
    free(actual);

    imprimir_eliminar_correcta(RUC);
}

void imprimir_busqueda_correcta(int RUC) {
    printf("Se encontró la denuncia de RUC: %d\n", RUC);
}

void imprimir_no_encontrado(int RUC) {
    printf("No existe esta denuncia o se digitó mal el RUC: %d\n", RUC);
}

struct Denuncias *buscarDenuncia(struct nodo_Denuncias *lista, int RUC) {
    while (lista != NULL) {
        if (lista->Denuncias != NULL && lista->Denuncias->RUC == RUC) {
            imprimir_busqueda_correcta(RUC);
            return lista->Denuncias;
        }
        lista = lista->sig;
    }

    imprimir_no_encontrado(RUC);
    return NULL;
}


void imprimir_modificacion_fallida(int RUC) {
    printf("No se pudo modificar la denuncia con RUC:%d. Verifique que exista en el sistema.\n", RUC);
}

void imprimir_modificacion_exitosa(int RUC, const char *nuevoTipo, const char *nuevaDescripcion, int nuevaFecha) {
    printf("La denuncia con RUC:%d ha sido modificada correctamente.\n", RUC);
    printf("Nuevo tipo de denunciante: %s\n", nuevoTipo);
    printf("Nueva descripción: %s\n", nuevaDescripcion);
    printf("Nueva fecha: %d\n", nuevaFecha);
}


int modificarDenuncia(struct nodo_Denuncias *lista, int RUC, const char *nuevoTipo, const char *nuevaDescripcion, int nuevaFecha) {
    if (!lista || !nuevoTipo || !nuevaDescripcion) {
        imprimir_modificacion_fallida(RUC);
        return -1;
    }

    while (lista) {
        if (lista->Denuncias && lista->Denuncias->RUC == RUC) {
            strcpy(lista->Denuncias->Tipo_denunciante, nuevoTipo);
            strncpy(lista->Denuncias->descripcion, nuevaDescripcion, sizeof(lista->Denuncias->descripcion) - 1);
            lista->Denuncias->fecha = nuevaFecha;
            imprimir_modificacion_exitosa(RUC, nuevoTipo, nuevaDescripcion, nuevaFecha);
            return 1;
        }
        lista = lista->sig;
    }
    imprimir_modificacion_fallida(RUC);
    return -1;
}

//MENU DENUNCIA
void imprimir_denuncia(struct Denuncias *d) {
    if(d == NULL)
        return;
    printf("\n------ Denuncia ------\n");
    printf("RUC: %d\n", d->RUC);
    printf("Tipo de denunciante: %s\n", d->Tipo_denunciante);
    printf("Descripción: %s\n", d->descripcion);
    printf("Fecha: %d\n", d->fecha);
}

void crearDenunciaMenu(void) {
    int RUC, fecha;
    char tipo[30], desc[256];
    printf("Ingrese el RUC: \n");
    scanf("%d", &RUC);
    getchar(); //WARNING
    printf("Ingrese el tipo de denunciante: \n");
    fgets(tipo, sizeof(tipo), stdin);
    tipo[strcspn(tipo, "\n")] = '\0';
    printf("Ingrese la descripción de la denuncia: \n");
    fgets(desc, sizeof(desc), stdin);
    desc[strcspn(desc, "\n")] = '\0';
    printf("Ingrese la fecha (ej. 20250608): \n");
    scanf("%d", &fecha);
    getchar(); //WARNING

    agregarDenuncia(&lista_denuncias, RUC, tipo, desc, fecha);
}

void eliminarDenunciaMenu(void) {
    int RUC;
    printf("Ingrese el RUC de la denuncia a eliminar: \n");
    scanf("%d", &RUC);
    getchar(); //WARNING
    eliminarDenuncia(&lista_denuncias, RUC);
}

void buscarDenunciaMenu(void) {
    int RUC;
    printf("Ingrese el RUC de la denuncia a buscar: \n");
    scanf("%d", &RUC);
    getchar(); //WARNING
    buscarDenuncia(lista_denuncias, RUC);
}


void modificarDenunciaMenu(void) {
    int RUC, nuevaFecha, opcion, ret;
    char nuevoTipo[30], nuevaDescripcion[256];
    struct Denuncias *denuncia = NULL;

    printf("Ingrese el RUC de la denuncia a modificar: \n");
    scanf("%d", &RUC);
    getchar(); //WARNING

    denuncia = buscarDenuncia(lista_denuncias, RUC);
    if (denuncia == NULL) {
        printf("\nDenuncia con RUC %d no encontrada.\n", RUC);
        return;
    }

    strcpy(nuevoTipo, denuncia->Tipo_denunciante);
    strcpy(nuevaDescripcion, denuncia->descripcion);
    nuevaFecha = denuncia->fecha;

    printf("\nValores actuales:\n");
    printf("Tipo de denunciante: %s\n", nuevoTipo);
    printf("Descripción: %s\n", nuevaDescripcion);
    printf("Fecha: %d\n", nuevaFecha);
    printf("\nSeleccione el campo a modificar:\n");
    printf("1. Modificar solo el tipo de denunciante\n");
    printf("2. Modificar solo la descripción\n");
    printf("3. Modificar solo la fecha\n");
    printf("4. Modificar todos los campos\n");
    printf("Opción: ");
    scanf("%d", &opcion);
    getchar(); //WARNING

    switch (opcion) {
        case 1:
            printf("Ingrese el nuevo tipo de denunciante: \n");
            fgets(nuevoTipo, sizeof(nuevoTipo), stdin);
            nuevoTipo[strcspn(nuevoTipo, "\n")] = '\0';
            break;
        case 2:
            printf("Ingrese la nueva descripción: \n");
            fgets(nuevaDescripcion, sizeof(nuevaDescripcion), stdin);
            nuevaDescripcion[strcspn(nuevaDescripcion, "\n")] = '\0';
            break;
        case 3:
            printf("Ingrese la nueva fecha (ej. 20250608): \n");
            scanf("%d", &nuevaFecha);
            getchar(); //WARNING
            break;
        case 4:
            printf("Ingrese el nuevo tipo de denunciante: \n");
            fgets(nuevoTipo, sizeof(nuevoTipo), stdin);
            nuevoTipo[strcspn(nuevoTipo, "\n")] = '\0';
            printf("Ingrese la nueva descripción: \n");
            fgets(nuevaDescripcion, sizeof(nuevaDescripcion), stdin);
            nuevaDescripcion[strcspn(nuevaDescripcion, "\n")] = '\0';
            printf("Ingrese la nueva fecha (ej. 20250608): \n");
            scanf("%d", &nuevaFecha);
            getchar(); //WARNING
            break;
        default:
            printf("OPCIÓN INVALIDA.\n");
            return;
    }

    ret = modificarDenuncia(lista_denuncias, RUC, nuevoTipo, nuevaDescripcion, nuevaFecha);
    if (ret == 1)
        printf("Denuncia modificada exitosamente.\n");
    else
        printf("La modificación falló. Verifique que el RUC exista.\n");
}

void menu_Denuncias(void) {
    int opcion;
    do {
        printf("\n----- MENU DENUNCIAS -----\n");
        printf("1. Crear (Agregar) Denuncia\n");
        printf("2. Eliminar Denuncia\n");
        printf("3. Buscar Denuncia\n");
        printf("4. Modificar Denuncia\n");
        printf("5. Listar Denuncias\n");
        printf("0. Volver al menú principal\n");
        printf("Seleccione una opción: ");
        scanf("%d", &opcion);
        getchar(); //WARNING
        switch(opcion) {
            case 1:
                crearDenunciaMenu();
                break;
            case 2:
                eliminarDenunciaMenu();
                break;
            case 3:
                buscarDenunciaMenu();
                break;
            case 4:
                modificarDenunciaMenu();
                break;
            case 5:
                printf("Listando denuncias...\n");
                break;
            case 0:
                break;
            default:
                printf("Opción inválida.\n");
        }
    } while(opcion != 0);
}
//FUNCIONES CARPETA
struct nodo_carpeta *lista_carpetas = NULL;

void imprimir_nueva_Carpeta(int RUC) {
    printf("Nueva carpeta agregada correctamente con RUC: %d\n", RUC);
}

void imprimir_error_Carpeta(int RUC) {
    printf("ERROR al ingresar RUC: %d o en alguna de los datos, vuelva a intentarlo.\n", RUC);
}

int agregarCarpeta(struct nodo_carpeta **lista, struct Carpeta *nueva) {
    struct nodo_carpeta *nuevoNodo;
    struct nodo_carpeta *actual;
   
    if (!lista || !nueva) {
        imprimir_error_Carpeta(nueva ? nueva->RUC : -1);
        return -1;
    }
    
    nuevoNodo = (struct nodo_carpeta *)malloc(sizeof(struct nodo_carpeta));
    if (!nuevoNodo) {
        imprimir_error_Carpeta(nueva->RUC);
        return -1;
    }
    
    nuevoNodo->carpeta = nueva;
    nuevoNodo->sig = NULL;
    nuevoNodo->ant = NULL;
    
    if (*lista == NULL) {
        *lista = nuevoNodo;
    } else {
        actual = *lista;
        while (actual->sig) {
            actual = actual->sig;
        }
        actual->sig = nuevoNodo;
        nuevoNodo->ant = actual;
    }

    imprimir_nueva_Carpeta(nueva->RUC);
    return 0;
}

void imprimir_busqueda_carpeta(int RUC) {
    printf("Se encontró la carpeta con RUC: %d.\n", RUC);
}

void imprimir_no_carpeta(int RUC) {
    printf("No existe esta carpeta o se digitó mal el RUC: %d.\n", RUC);
}

struct Carpeta *buscarCarpeta(struct nodo_carpeta *lista, int RUC) {
    while (lista) {
        if (lista->carpeta && lista->carpeta->RUC == RUC) {
            imprimir_busqueda_carpeta(RUC);
            return lista->carpeta;
        }
        lista = lista->sig;
    }

    imprimir_no_carpeta(RUC);
    return NULL;
}

void imprimir_eliminar_carpeta(int RUC) {
    printf("La eliminación de la carpeta con RUC: %d fue exitosa.\n", RUC);
}
void imprimir_carpeta_fallo(int RUC) {
    printf("No existe esta carpeta o el RUC: %d no fue encontrado.\n", RUC);
}

void eliminarCarpeta(struct nodo_carpeta **lista, struct Carpeta *carpeta_eliminada) {
    struct nodo_carpeta*actual;
    int RUC;
    
    if (*lista == NULL || carpeta_eliminada == NULL) {
        imprimir_carpeta_fallo(carpeta_eliminada ? carpeta_eliminada->RUC : -1);
        return;
    }
    
    actual = *lista;
    while (actual) {
        if (actual->carpeta == carpeta_eliminada) {
            RUC = actual->carpeta->RUC;

            if (actual->ant) {
                actual->ant->sig = actual->sig;
            } else {
                *lista = actual->sig;
            }

            if (actual->sig) {
                actual->sig->ant = actual->ant;
            }
            if (actual ->carpeta->imputados) {
                free(actual->carpeta->imputados);
            }
            
            free(actual->carpeta);
            free(actual);

            imprimir_eliminar_carpeta(RUC);
            return;
        }
        actual = actual->sig;
    }

    imprimir_carpeta_fallo(carpeta_eliminada->RUC);
}

void imprimir_modificacion_fallida_carpeta(int RUC) {
    printf("No se pudo modificar la carpeta con RUC: %d. Verifique que exista en el sistema.\n", RUC);
}

void imprimir_modificacion_exitosa_carpeta(int RUC, const char *testigo, const char *victima, const char *resolucion, const char *declaracion, const char *pruebas) {
    printf("La carpeta con RUC: %d ha sido modificada correctamente.\n", RUC);
    printf("Nuevo testigo: %s\n", testigo);
    printf("Nueva víctima: %s\n", victima);
    printf("Nueva resolución: %s\n", resolucion);
    printf("Nueva declaración: %s\n", declaracion);
    printf("Nueva pruebas: %s\n", pruebas);
}

int modificarCarpeta(struct nodo_carpeta *lista, struct Carpeta *nueva) {
    
    struct nodo_carpeta*actual;
    
    if (!lista || !nueva) {
        imprimir_modificacion_fallida_carpeta(nueva ? nueva->RUC : -1);
        return -1;
    }

    actual = lista;

    while (actual) {
        if (actual->carpeta->RUC == nueva->RUC) {
            
            if (nueva->testigos && actual->carpeta->testigos) {
                strcpy(actual->carpeta->testigos, nueva->testigos);
            }
            if (nueva->victimas && actual->carpeta->victimas) {
                strcpy(actual->carpeta->victimas, nueva->victimas);
            }
            if (nueva->resolucion && actual->carpeta->resolucion) {
                strcpy(actual->carpeta->resolucion, nueva->resolucion);
            }
            if (nueva->declaraciones && actual->carpeta->declaraciones) {
                strcpy(actual->carpeta->declaraciones, nueva->declaraciones);
            }
            if (nueva->pruebas && actual->carpeta->pruebas) {
                strcpy(actual->carpeta->pruebas, nueva->pruebas);
            }

            imprimir_modificacion_exitosa_carpeta(nueva->RUC, nueva->testigos, nueva->victimas, nueva->resolucion, nueva->declaraciones, nueva->pruebas);
            return 1;
        }
        actual = actual->sig;
    }

    imprimir_modificacion_fallida_carpeta(nueva->RUC);
    return -1;
}
//menu de carpeta
void imprimir_carpeta(struct Carpeta *c) {
    if (!c)
        return;
    printf("\n------ Datos de la Carpeta ------\n");
    printf("RUC: %d\n", c->RUC);
    printf("Testigos: %s\n", c->testigos);
    printf("Víctimas: %s\n", c->victimas);
    printf("Resolución: %s\n", c->resolucion);
    printf("Declaraciones: %s\n", c->declaraciones);
    printf("Pruebas: %s\n", c->pruebas);
}

void crearCarpetaMenu(void) {
    struct Carpeta *nueva = malloc(sizeof(struct Carpeta));
    if (!nueva) {
         printf("Error al asignar memoria para la nueva carpeta.\n");
         return;
    }
    printf("\nIngrese el RUC de la carpeta: ");
    scanf("%d", &nueva->RUC);
    getchar(); //WARNING
    printf("Ingrese los testigos: ");
    fgets(nueva->testigos, sizeof(nueva->testigos), stdin);
    nueva->testigos[strcspn(nueva->testigos, "\n")] = '\0';
    printf("Ingrese las víctimas: ");
    fgets(nueva->victimas, sizeof(nueva->victimas), stdin);
    nueva->victimas[strcspn(nueva->victimas, "\n")] = '\0';
    printf("Ingrese la resolución: ");
    fgets(nueva->resolucion, sizeof(nueva->resolucion), stdin);
    nueva->resolucion[strcspn(nueva->resolucion, "\n")] = '\0';
    printf("Ingrese las declaraciones: ");
    fgets(nueva->declaraciones, sizeof(nueva->declaraciones), stdin);
    nueva->declaraciones[strcspn(nueva->declaraciones, "\n")] = '\0';
    printf("Ingrese las pruebas: ");
    fgets(nueva->pruebas, sizeof(nueva->pruebas), stdin);
    nueva->pruebas[strcspn(nueva->pruebas, "\n")] = '\0';

    agregarCarpeta(&lista_carpetas, nueva);
}

void modificarCarpetaMenu(void) {
    int ruc, opcion;
    char password[20];
    struct nodo_carpeta *actual;
    struct Carpeta *temp;
    
    printf("Acceso restringido. Ingrese la contraseña: ");
    fgets(password, sizeof(password), stdin);
    password[strcspn(password, "\n")] = '\0';
    
    if (strcmp(password, "admin123") != 0) {
         printf("Contraseña incorrecta. Acceso denegado.\n");
         return;
    }
    printf("Ingrese RUC de la carpeta a modificar: ");
    scanf("%d", &ruc);
    getchar(); //WARNING

    actual = lista_carpetas;
    
    while(actual && actual->carpeta->RUC != ruc)
         actual = actual->sig;
    if (actual == NULL) {
         printf("Carpeta no encontrada.\n");
         return;
    }

    temp = (struct Carpeta*)malloc(sizeof(struct Carpeta));
    if (!temp) {
         printf("Error en memoria.\n");
         return;
    }

    temp->RUC = actual->carpeta->RUC;
    
    strcpy(temp->testigos, actual->carpeta->testigos);
    strcpy(temp->victimas, actual->carpeta->victimas);
    strcpy(temp->resolucion, actual->carpeta->resolucion);
    strcpy(temp->declaraciones, actual->carpeta->declaraciones);
    strcpy(temp->pruebas, actual->carpeta->pruebas);

    printf("\nValores actuales:\n");
    printf("Testigos: %s\n", temp->testigos);
    printf("Victimas: %s\n", temp->victimas);
    printf("Resolucion: %s\n", temp->resolucion);
    printf("Declaraciones: %s\n", temp->declaraciones);
    printf("Pruebas: %s\n", temp->pruebas);

    printf("\nSeleccione el campo a modificar:\n");
    printf("1. Modificar solo testigos\n");
    printf("2. Modificar solo victimas\n");
    printf("3. Modificar solo resolucion\n");
    printf("4. Modificar solo declaraciones\n");
    printf("5. Modificar solo pruebas\n");
    printf("6. Modificar todos los campos\n");
    printf("Opcion: ");
    scanf("%d", &opcion);
    getchar(); //WARNING
    switch (opcion) {
         case 1:
              printf("Ingrese nuevos testigos: \n");
              fgets(temp->testigos, sizeof(temp->testigos), stdin);
              temp->testigos[strcspn(temp->testigos, "\n")] = '\0';
              break;
         case 2:
              printf("Ingrese nuevas victimas: \n");
              fgets(temp->victimas, sizeof(temp->victimas), stdin);
              temp->victimas[strcspn(temp->victimas, "\n")] = '\0';
              break;
         case 3:
              printf("Ingrese nueva resolucion: \n");
              fgets(temp->resolucion, sizeof(temp->resolucion), stdin);
              temp->resolucion[strcspn(temp->resolucion, "\n")] = '\0';
              break;
         case 4:
              printf("Ingrese nuevas declaraciones: \n");
              fgets(temp->declaraciones, sizeof(temp->declaraciones), stdin);
              temp->declaraciones[strcspn(temp->declaraciones, "\n")] = '\0';
              break;
         case 5:
              printf("Ingrese nuevas pruebas: \n");
              fgets(temp->pruebas, sizeof(temp->pruebas), stdin);
              temp->pruebas[strcspn(temp->pruebas, "\n")] = '\0';
              break;
         case 6:
              printf("Ingrese nuevos testigos: \n");
              fgets(temp->testigos, sizeof(temp->testigos), stdin);
              temp->testigos[strcspn(temp->testigos, "\n")] = '\0';
              printf("Ingrese nuevas victimas: \n");
              fgets(temp->victimas, sizeof(temp->victimas), stdin);
              temp->victimas[strcspn(temp->victimas, "\n")] = '\0';
              printf("Ingrese nueva resolucion: \n");
              fgets(temp->resolucion, sizeof(temp->resolucion), stdin);
              temp->resolucion[strcspn(temp->resolucion, "\n")] = '\0';
              printf("Ingrese nuevas declaraciones: \n");
              fgets(temp->declaraciones, sizeof(temp->declaraciones), stdin);
              temp->declaraciones[strcspn(temp->declaraciones, "\n")] = '\0';
              printf("Ingrese nuevas pruebas: \n");
              fgets(temp->pruebas, sizeof(temp->pruebas), stdin);
              temp->pruebas[strcspn(temp->pruebas, "\n")] = '\0';
              break;
         default:
              printf("OPCIÓN INVALIDA\n");
              free(temp);
              return;
    }

    strcpy(actual->carpeta->testigos, temp->testigos);
    strcpy(actual->carpeta->victimas, temp->victimas);
    strcpy(actual->carpeta->resolucion, temp->resolucion);
    strcpy(actual->carpeta->declaraciones, temp->declaraciones);
    strcpy(actual->carpeta->pruebas, temp->pruebas);
    printf("Carpeta modificada exitosamente.\n");
    free(temp);
}

void buscarCarpetaMenu(void) {
    int ruc;
    struct nodo_carpeta *actual;
    printf("Ingrese el RUC de la carpeta a buscar: \n");
    scanf("%d", &ruc);
    getchar(); //WARNING
    actual  = lista_carpetas; //ERROR PRINCIPAL PQ NO ENTRA LISTA CARPETA
    while(actual != NULL) {
         if(actual->carpeta->RUC == ruc) { //ERROR PQ NO TIENE ACTUAL LOS DATOS DE LISTA CARPETA
              printf("Carpeta encontrada:\n");
              imprimir_carpeta(actual->carpeta); //ERROR NO SE PUEDE IMPRIMIR
              return;
         }
         actual = actual->sig; //ERROR ACTUAL NO TIENE SIG
    }
    printf("No se encontró carpeta con RUC %d.\n", ruc);
}

void eliminarCarpetaMenu(void) {
    int ruc;
    struct nodo_carpeta *actual;
    struct nodo_carpeta *anterior = NULL;
    printf("Ingrese el RUC de la carpeta a eliminar: \n");
    scanf("%d", &ruc);
    getchar(); //WARNING
    
    actual = lista_carpetas;
    while(actual && actual->carpeta->RUC != ruc) {
         anterior = actual;
         actual = actual->sig;
    }
    if(actual == NULL) {
         printf("No se encontró carpeta con RUC %d.\n", ruc);
         return;
    }
    if(anterior == NULL)
         lista_carpetas = actual->sig;
    else
         anterior->sig = actual->sig;
    if(actual->sig)
         actual->sig->ant = anterior;
    free(actual->carpeta);
    free(actual);
    printf("Carpeta eliminada exitosamente.\n");
}

void menuCarpetas(void) {
    char pass[20];
    int opcion;
    printf("Ingrese la contraseña para acceder a las Carpetas Investigativas: \n");
    fgets(pass, sizeof(pass), stdin);
    pass[strcspn(pass, "\n")] = '\0';
    if(strcmp(pass, "admin123") != 0) {
         printf("Contraseña incorrecta. Acceso denegado.\n");
         return;
    }

    do {
         printf("\n==== MENU CARPETAS ====\n");
         printf("1. Crear Carpeta\n");
         printf("2. Modificar Carpeta\n");
         printf("3. Buscar Carpeta\n");
         printf("4. Eliminar Carpeta\n");
         printf("0. Volver\n");
         printf("Seleccione una opcion: ");
         scanf("%d", &opcion);
         getchar(); //WARNING
         switch(opcion) {
              case 1:
                   crearCarpetaMenu();
                   break;
              case 2:
                   modificarCarpetaMenu();
                   break;
              case 3:
                   buscarCarpetaMenu();
                   break;
              case 4:
                   eliminarCarpetaMenu();
                   break;
              case 0:
                   printf("Volviendo al menu principal...\n");
                   break;
              default:
                   printf("Opcion invalida.\n");
         }
    } while(opcion != 0);
}

//funciones de imputados
struct nodo_imputado *arbol_imputados = NULL;

struct imputado* crear_imputado(char* rut, char* nombre, char* estadoProcesal, char* medidasCautelares, char* defensor) {
    struct imputado* nuevo = malloc(sizeof(struct imputado));
    if (nuevo == NULL)
        return NULL;
    strncpy(nuevo->rut, rut, sizeof(nuevo->rut)-1);
    nuevo->rut[sizeof(nuevo->rut)-1] = '\0';
    strncpy(nuevo->nombre, nombre, sizeof(nuevo->nombre)-1);
    nuevo->nombre[sizeof(nuevo->nombre)-1] = '\0';
    strncpy(nuevo->estadoProcesal, estadoProcesal, sizeof(nuevo->estadoProcesal)-1);
    nuevo->estadoProcesal[sizeof(nuevo->estadoProcesal)-1] = '\0';
    strncpy(nuevo->medidasCautelares, medidasCautelares, sizeof(nuevo->medidasCautelares)-1);
    nuevo->medidasCautelares[sizeof(nuevo->medidasCautelares)-1] = '\0';
    strncpy(nuevo->defensor, defensor, sizeof(nuevo->defensor)-1);
    nuevo->defensor[sizeof(nuevo->defensor)-1] = '\0';
    return nuevo;
}

struct nodo_imputado* crear_nodo_imputado(struct imputado* nuevo_imputado) {
    struct nodo_imputado* nodo = malloc(sizeof(struct nodo_imputado));
    if (nodo == NULL)
        return NULL;
    nodo->datos = nuevo_imputado;
    nodo->izq = NULL;
    nodo->der = NULL;
    return nodo;
}

void imprimir_agregar_imputado_exitoso(char *rut) {
    printf("El imputado con RUT: %s ha sido agregado correctamente.\n", rut);
}

void imprimir_error_agregar_imputado(char *rut) {
    printf("No se pudo agregar el imputado con RUT: %s. Hubo un error en la asignación de memoria.\n", rut);
}


struct nodo_imputado* agregar_imputado(struct nodo_imputado* raiz, char* rut, char* nombre, char* estadoProcesal, char* medidasCautelares, char* defensor) {
    struct imputado *nuevo_imputado;
    struct nodo_imputado* nuevo_nodo;
    int comparacion;

    if (raiz == NULL) {
        nuevo_imputado = crear_imputado(rut, nombre, estadoProcesal, medidasCautelares, defensor);
        if (nuevo_imputado == NULL) {
            imprimir_error_agregar_imputado(rut);
            return NULL;
        }
        nuevo_nodo = crear_nodo_imputado(nuevo_imputado);
        if (nuevo_nodo == NULL) {
            free(nuevo_imputado);
            imprimir_error_agregar_imputado(rut);
            return NULL;
        }
        imprimir_agregar_imputado_exitoso(rut);
        return nuevo_nodo;
    }

    comparacion = strcmp(rut, raiz->datos->rut);

    if (comparacion < 0) {
        raiz->izq = agregar_imputado(raiz->izq, rut, nombre, estadoProcesal, medidasCautelares, defensor);
    }
    if (comparacion > 0) {
        raiz->der = agregar_imputado(raiz->der, rut, nombre, estadoProcesal, medidasCautelares, defensor);
    }

    return raiz;
}

void imprimir_busqueda_imputado_exitoso(char *rut) {
    printf("Imputado encontrado con RUT: %s.\n", rut);
}

void imprimir_busqueda_imputado_fallida(char *rut) {
    printf("No se encontró un imputado con RUT: %s.\n", rut);
}

struct nodo_imputado* buscar_imputado(struct nodo_imputado* raiz, char* rut) {
    int comparacion;
    
    if (raiz == NULL) {
        imprimir_busqueda_imputado_fallida(rut);
        return NULL;
    }

    comparacion = strcmp(rut, raiz->datos->rut);

    if (comparacion == 0) {
        imprimir_busqueda_imputado_exitoso(rut);
        return raiz;
    }
    else if (comparacion < 0) {
        return buscar_imputado(raiz->izq, rut);
    }
    else {
        return buscar_imputado(raiz->der, rut);
    }
}

struct nodo_imputado* encontrar_minimo(struct nodo_imputado* nodo) {
    while (nodo->izq != NULL) {
        nodo = nodo->izq;
    }
    return nodo;
}
void imprimir_eliminar_imputado_exitoso(char *rut) {
    printf("El imputado con RUT: %s ha sido eliminado correctamente.\n", rut);
}

void imprimir_eliminar_imputado_fallida(char *rut) {
    printf("No se encontró un imputado con RUT: %s para eliminar.\n", rut);
}

struct nodo_imputado* eliminar_imputado(struct nodo_imputado* raiz, char* rut) {
    int comparacion;
    struct nodo_imputado* temp;
    
    if (raiz == NULL) {
        imprimir_eliminar_imputado_fallida(rut);
        return raiz;
    }

    comparacion = strcmp(rut, raiz->datos->rut);

    if (comparacion < 0) {
        raiz->izq = eliminar_imputado(raiz->izq, rut);
    }
    else if (comparacion > 0) {
        raiz->der = eliminar_imputado(raiz->der, rut);
    }
    else {
        imprimir_eliminar_imputado_exitoso(rut);

        if (raiz->izq == NULL) {
            struct nodo_imputado* temp = raiz->der;
            free(raiz->datos);
            free(raiz);
            return temp;
        }
        else if (raiz->der == NULL) {
            struct nodo_imputado* temp = raiz->izq;
            free(raiz->datos);
            free(raiz);
            return temp;
        }

        temp = encontrar_minimo(raiz->der);
        free(raiz->datos);
        raiz->datos = (struct imputado*)malloc(sizeof(struct imputado));
        memcpy(raiz->datos, temp->datos, sizeof(struct imputado));
        raiz->der = eliminar_imputado(raiz->der, temp->datos->rut);
    }

    return raiz;
}

void imprimir_modificacion_exitosa_imputado(const char *rut, const char *nombre, const char *estado_procesal, const char *medidasCautelares, const char *defensor) {
    printf("La carpeta con RUT: %s ha sido modificada correctamente.\n", rut);
    printf("Nuevo nombre: %s\n", nombre);
    printf("Nuevo estado procesal: %s\n", estado_procesal);
    printf("Nuevas medidas cautelares: %s\n", medidasCautelares);
    printf("Nuevo defensor: %s\n", defensor);
}

void imprimir_modificacion_imputado_fallida(char *rut) {
    printf("No se pudo modificar el imputado con RUT: %s porque no existe en el sistema.\n", rut);
}

int modificar_imputado(struct nodo_imputado* raiz, char* rut, char* nuevo_nombre, char* nuevo_estadoProcesal, char* nuevas_medidasCautelares, char* nuevo_defensor) {
    struct nodo_imputado* nodo = buscar_imputado(raiz, rut);

    if (nodo == NULL) {
        imprimir_modificacion_imputado_fallida(rut);
        return -1;
    }

    if (nuevo_nombre != NULL) {
        strncpy(nodo->datos->nombre, nuevo_nombre, 29);
        nodo->datos->nombre[29] = '\0';
    }
    if (nuevo_estadoProcesal != NULL) {
        strncpy(nodo->datos->estadoProcesal, nuevo_estadoProcesal, 29);
        nodo->datos->estadoProcesal[29] = '\0';
    }
    if (nuevas_medidasCautelares != NULL) {
        strncpy(nodo->datos->medidasCautelares, nuevas_medidasCautelares, 49);
        nodo->datos->medidasCautelares[49] = '\0';
    }
    if (nuevo_defensor != NULL) {
        strncpy(nodo->datos->defensor, nuevo_defensor, 29);
        nodo->datos->defensor[29] = '\0';
    }

    imprimir_modificacion_exitosa_imputado(rut,nuevo_nombre,nuevo_estadoProcesal,nuevas_medidasCautelares,nuevo_defensor);
    return 1;
}
//MENU IMPUTADO
void modificarImputadoMenu(void) {
    char rut[15], nuevo_nombre[30], nuevo_estado[30], nuevas_medidas[50], nuevo_defensor[30];
    int opcion;
    struct nodo_imputado *node;
    
    printf("Ingrese RUT del imputado a modificar: ");
    fgets(rut, sizeof(rut), stdin);
    rut[strcspn(rut, "\n")] = '\0';
    node = buscar_imputado(arbol_imputados, rut);
    
    if(node == NULL)
        return;
    strcpy(nuevo_nombre, node->datos->nombre);
    strcpy(nuevo_estado, node->datos->estadoProcesal);
    strcpy(nuevas_medidas, node->datos->medidasCautelares);
    strcpy(nuevo_defensor, node->datos->defensor);
    printf("\nValores actuales:\n");
    printf("Nombre: %s\n", nuevo_nombre);
    printf("Estado Procesal: %s\n", nuevo_estado);
    printf("Medidas Cautelares: %s\n", nuevas_medidas);
    printf("Defensor: %s\n", nuevo_defensor);
    printf("\nSeleccione el campo a modificar:\n");
    printf("1. Modificar solo el Nombre\n");
    printf("2. Modificar solo el Estado Procesal\n");
    printf("3. Modificar solo las Medidas Cautelares\n");
    printf("4. Modificar solo el Defensor\n");
    printf("5. Modificar todos los campos\n");
    printf("Opción: ");
    scanf("%d", &opcion);
    getchar(); //WARNING
    switch(opcion) {
        case 1:
            printf("Ingrese el nuevo Nombre: \n");
            fgets(nuevo_nombre, sizeof(nuevo_nombre), stdin);
            nuevo_nombre[strcspn(nuevo_nombre, "\n")] = '\0';
            break;
        case 2:
            printf("Ingrese el nuevo Estado Procesal: \n");
            fgets(nuevo_estado, sizeof(nuevo_estado), stdin);
            nuevo_estado[strcspn(nuevo_estado, "\n")] = '\0';
            break;
        case 3:
            printf("Ingrese las nuevas Medidas Cautelares: \n");
            fgets(nuevas_medidas, sizeof(nuevas_medidas), stdin);
            nuevas_medidas[strcspn(nuevas_medidas, "\n")] = '\0';
            break;
        case 4:
            printf("Ingrese el nuevo Defensor: \n");
            fgets(nuevo_defensor, sizeof(nuevo_defensor), stdin);
            nuevo_defensor[strcspn(nuevo_defensor, "\n")] = '\0';
            break;
        case 5:
            printf("Ingrese el nuevo Nombre: \n");
            fgets(nuevo_nombre, sizeof(nuevo_nombre), stdin);
            nuevo_nombre[strcspn(nuevo_nombre, "\n")] = '\0';
            printf("Ingrese el nuevo Estado Procesal: \n");
            fgets(nuevo_estado, sizeof(nuevo_estado), stdin);
            nuevo_estado[strcspn(nuevo_estado, "\n")] = '\0';
            printf("Ingrese las nuevas Medidas Cautelares: \n");
            fgets(nuevas_medidas, sizeof(nuevas_medidas), stdin);
            nuevas_medidas[strcspn(nuevas_medidas, "\n")] = '\0';
            printf("Ingrese el nuevo Defensor: \n");
            fgets(nuevo_defensor, sizeof(nuevo_defensor), stdin);
            nuevo_defensor[strcspn(nuevo_defensor, "\n")] = '\0';
            break;
        default:
            printf("Opción inválida.\n");
            return;
    }
    modificar_imputado(arbol_imputados, rut, nuevo_nombre, nuevo_estado, nuevas_medidas, nuevo_defensor);
}

void menuImputados(void) {
    int opcion;
    char rut[15], nombre[30], estado[30], medidas[50], defensor[30];
    do {
        printf("\n==== MENU IMPUTADOS ====\n");
        printf("1. Agregar Imputado\n");
        printf("2. Buscar Imputado\n");
        printf("3. Modificar Imputado\n");
        printf("4. Eliminar Imputado\n");
        printf("0. Volver\n");
        printf("Seleccione una opción: ");
        scanf("%d", &opcion);
        getchar(); //WARNING
        switch(opcion) {
            case 1:
                printf("Ingrese RUT: \n");
                fgets(rut, sizeof(rut), stdin);
                rut[strcspn(rut, "\n")] = '\0';
                printf("Ingrese Nombre: \n");
                fgets(nombre, sizeof(nombre), stdin);
                nombre[strcspn(nombre, "\n")] = '\0';
                printf("Ingrese Estado Procesal: \n");
                fgets(estado, sizeof(estado), stdin);
                estado[strcspn(estado, "\n")] = '\0';
                printf("Ingrese Medidas Cautelares: \n");
                fgets(medidas, sizeof(medidas), stdin);
                medidas[strcspn(medidas, "\n")] = '\0';
                printf("Ingrese Defensor: ");
                fgets(defensor, sizeof(defensor), stdin);
                defensor[strcspn(defensor, "\n")] = '\0';
                arbol_imputados = agregar_imputado(arbol_imputados, rut, nombre, estado, medidas, defensor);
                break;
            case 2:
                printf("Ingrese RUT a buscar: \n");
                fgets(rut, sizeof(rut), stdin);
                rut[strcspn(rut, "\n")] = '\0';
                buscar_imputado(arbol_imputados, rut);
                break;
            case 3:
                modificarImputadoMenu();
                break;
            case 4:
                printf("Ingrese RUT del imputado a eliminar: \n");
                fgets(rut, sizeof(rut), stdin);
                rut[strcspn(rut, "\n")] = '\0';
                arbol_imputados = eliminar_imputado(arbol_imputados, rut);
                break;
            case 0:
                printf("Volviendo al menú principal...\n");
                break;
            default:
                printf("Opción inválida. Intente de nuevo.\n");
                break;
        }
    } while(opcion != 0);
}
//Funciones de causa
struct causa *lista_causas = NULL;

void intercambiarCausas(struct causa *a, struct causa *b) {
    struct causa temp = *a;
    *a = *b;
    *b = temp;
}

void priorizarCausas(struct causa causas[], int n) {
    int j, i;
    for ( i = 0; i < n - 1; i++) {
        for ( j = 0; j < n - i - 1; j++) {
            if (causas[j].urgencia < causas[j + 1].urgencia) {
                intercambiarCausas(&causas[j], &causas[j + 1]);
            }
        }
    }
}

void imprimir_agregar_causa_exitoso(int RUC) {
    printf("\nLa causa con RUC: %d ha sido agregada correctamente.\n", RUC);
}
void imprimir_error_agregar_causa(int RUC) {
    printf("No se pudo agregar la causa con RUC: %d. Hubo un error en la asignación de memoria.\n", RUC);
}

struct causa* agregar_causa(struct causa **lista, int RUC, int urgencia, const char *estado_causa) {
    struct causa *nueva_causa = (struct causa*)malloc(sizeof(struct causa));
    int maxCausas = 100;
    if (!nueva_causa) {
        imprimir_error_agregar_causa(RUC);
        return NULL;
    }
    nueva_causa->RUC = RUC;
    nueva_causa->urgencia = urgencia;
    strncpy(nueva_causa->estado_causa, estado_causa, sizeof(nueva_causa->estado_causa)-1);
    nueva_causa->estado_causa[sizeof(nueva_causa->estado_causa)-1] = '\0';
    nueva_causa->denuncia = NULL;
    nueva_causa->carpeta = NULL;
    nueva_causa->imputado = NULL;
    nueva_causa->fiscal = NULL;
    nueva_causa->carpeta = *lista;
    *lista = nueva_causa;
    priorizarCausas(*lista,maxCausas);
    imprimir_agregar_causa_exitoso(RUC);
    return nueva_causa;
}
void imprimir_busqueda_causa_exitoso(int RUC) {
    printf("Causa encontrada con RUC: %d.\n", RUC);
}
void imprimir_busqueda_causa_fallida(int RUC) {
    printf("No se encontró una causa con RUC: %d.\n", RUC);
}

struct causa* buscar_causa(struct causa *lista, int RUC) {
    while (lista) {
        if (lista->RUC == RUC) {
            imprimir_busqueda_causa_exitoso(RUC);
            return lista;
        }
        lista = lista->carpeta;
    }

    imprimir_busqueda_causa_fallida(RUC);
    return NULL;
}

void imprimir_modificacion_causa_fallida(int RUC) {
    printf("\nNo se pudo modificar la causa con RUC: %d porque no existe en el sistema.\n", RUC);
}
void imprimir_modificacion_causa(int RUC,int nueva_urgencia,const char *nuevo_estado) {
printf("n=== Modificación de Causa ===\n");
    printf("RUC: %d\n", RUC);
    printf("Nueva urgencia: %d\n",nueva_urgencia);
    printf("Nuevo estado: %s\n", nuevo_estado);
}

int modificar_causa(struct causa *lista, int RUC, int nueva_urgencia, const char *nuevo_estado) {
    struct causa *causa = buscar_causa(lista, RUC);

    if (!causa) {
        imprimir_modificacion_causa_fallida(RUC);
        return -1;
    }

    causa->urgencia = nueva_urgencia;
    strncpy(causa->estado_causa, nuevo_estado, sizeof(causa->estado_causa) - 1);
    causa->estado_causa[sizeof(causa->estado_causa) - 1] = '\0';

    imprimir_modificacion_causa(RUC,nueva_urgencia,nuevo_estado);
    return 1;
}

void imprimir_eliminar_causa_exitoso(int RUC) {
    printf("La causa con RUC: %d ha sido eliminada correctamente.\n", RUC);
}
void imprimir_eliminar_causa_fallida(int RUC) {
    printf("No se encontró una causa con RUC: %d para eliminar.\n", RUC);
}

void eliminar_causa(struct causa **lista, int RUC) {
    struct causa *actual = *lista;
    struct causa *anterior = NULL;

    while (actual) {
        if (actual->RUC == RUC) {
            if (anterior) {
                anterior->carpeta = actual->carpeta;
            } else {
                *lista = actual->carpeta;
            }

            free(actual);
            imprimir_eliminar_causa_exitoso(RUC);
            return;
        }
        anterior = actual;
        actual = actual->carpeta;
    }

    imprimir_eliminar_causa_fallida(RUC);
}

//MENU CAUSA
void modificarCausaMenu(void) {
    struct causa *c = buscar_causa;
    int RUC, opcion, new_urgencia;
    char new_estado[50], current_estado[50];
    printf("Ingrese el RUC de la causa a modificar: ");
    scanf("%d", &RUC);
    getchar(); //WARNING
    c = buscar_causa(lista_causas, RUC);
    if(c == NULL)
        return;
    new_urgencia = c->urgencia;
    strncpy(current_estado, c->estado_causa, sizeof(current_estado));
    current_estado[sizeof(current_estado)-1] = '\0';
    printf("\nValores actuales:\n");
    printf("Urgencia: %d\n", c->urgencia);
    printf("Estado: %s\n", current_estado);
    printf("\nSeleccione el campo a modificar:\n");
    printf("1. Modificar solo la urgencia\n");
    printf("2. Modificar solo el estado\n");
    printf("3. Modificar ambos\n");
    printf("Opcion: ");
    scanf("%d", &opcion);
    getchar(); //WARNING
    switch(opcion) {
        case 1:
            printf("Ingrese la nueva urgencia: \n");
            scanf("%d", &new_urgencia);
            getchar(); //WARNING
            strncpy(new_estado, current_estado, sizeof(new_estado));
            new_estado[sizeof(new_estado)-1] = '\0';
            break;
        case 2:
            printf("Ingrese el nuevo estado: ");
            fgets(new_estado, sizeof(new_estado), stdin);
            new_estado[strcspn(new_estado, "\n")] = '\0';
            break;
        case 3:
            printf("Ingrese la nueva urgencia: \n");
            scanf("%d", &new_urgencia);
            getchar(); //WARNING
            printf("Ingrese el nuevo estado: \n");
            fgets(new_estado, sizeof(new_estado), stdin);
            new_estado[strcspn(new_estado, "\n")] = '\0';
            break;
        default:
            printf("Opcion invalida.\n");
            return;
    }
    modificar_causa(lista_causas, RUC, new_urgencia, new_estado);
}

//FUCIONES FISCAL
struct fiscal* crear_fiscal(char* nombre_fiscal, char* diligencias) {
    struct fiscal* nuevo = (struct fiscal*)malloc(sizeof(struct fiscal));
    if (nuevo == NULL) {
        return NULL;
    }

    strncpy(nuevo->nombre_fiscal, nombre_fiscal, 29);
    nuevo->nombre_fiscal[29] = '\0';
    strncpy(nuevo->diligencias, diligencias, 99);
    nuevo->diligencias[99] = '\0';

    return nuevo;
}

struct fiscal* agregar_fiscal(char* nombre_fiscal, char* diligencias) { //ERROR POR PARAMETRO
    struct fiscal* nuevo_fiscal;
    if (nombre_fiscal == NULL || diligencias == NULL) {
        return NULL;
    }
    nuevo_fiscal = crear_fiscal(nombre_fiscal, diligencias);

    if (nuevo_fiscal == NULL) {
        return NULL;
    }
    return nuevo_fiscal;
}

struct fiscal* buscar_fiscal(struct fiscal** fiscales, int cantidad, char* nombre_fiscal) {
    int i;
    if (fiscales == NULL || nombre_fiscal == NULL || cantidad <= 0) {
        return NULL;
    }
    for (i = 0; i < cantidad; i++) {
        if (fiscales[i] != NULL && strcmp(fiscales[i]->nombre_fiscal, nombre_fiscal) == 0) {
            return fiscales[i];
        }
    }
    return NULL;
}
struct fiscal* buscar_fiscal_en_causa(struct causa* c) {
    if (c == NULL) {
        return NULL;
    }
    return c->fiscal;
}
void modificarFiscalEnCausa(struct causa *c) {
    char nuevoNombre[30], nuevasDiligencias[100];
    printf("Ingrese nuevo nombre fiscal: \n");
    fgets(nuevoNombre, sizeof(nuevoNombre), stdin);
    nuevoNombre[strcspn(nuevoNombre, "\n")] = '\0';
    printf("Ingrese nuevas diligencias: \n");
    fgets(nuevasDiligencias, sizeof(nuevasDiligencias), stdin);
    nuevasDiligencias[strcspn(nuevasDiligencias, "\n")] = '\0';
    strncpy(c->fiscal->nombre_fiscal, nuevoNombre, sizeof(c->fiscal->nombre_fiscal)-1);
    c->fiscal->nombre_fiscal[sizeof(c->fiscal->nombre_fiscal)-1] = '\0';
    strncpy(c->fiscal->diligencias, nuevasDiligencias, sizeof(c->fiscal->diligencias)-1);
    c->fiscal->diligencias[sizeof(c->fiscal->diligencias)-1] = '\0';
    printf("Fiscal modificado correctamente.\n");
}

void eliminarFiscalEnCausa(struct causa *c) {
    if (c == NULL)
        return;
    if (c->fiscal != NULL) {
        free(c->fiscal);
        c->fiscal = NULL;
        printf("Fiscal eliminado correctamente.\n");
    } else {
        printf("No hay fiscal asignado.\n");
    }
}
//MENU FISCAL
void buscarFiscalEnCausaMenu(void) {
    struct causa *c;
    int RUC, opcion;
    char nombre[30], diligencias[100];

    printf("Ingrese RUC de la causa: ");
    scanf("%d", &RUC);
    getchar(); //WARNING

    c = buscar_causa(lista_causas, RUC);
    if (c == NULL) {
        printf("Causa no encontrada.\n");
        return;
    }

    if (c->fiscal) {
        printf("Fiscal asignado a la causa:\n");
        printf("Nombre Fiscal: %s\n", c->fiscal->nombre_fiscal);
        printf("Diligencias: %s\n", c->fiscal->diligencias);

        printf("Seleccione opción:\n");
        printf("1. Modificar Fiscal\n");
        printf("2. Eliminar Fiscal\n");
        printf("0. Cancelar\n");
        scanf("%d", &opcion);
        getchar(); //WARNING

        switch (opcion) {
            case 1:
                modificarFiscalEnCausa(c);
                break;
            case 2:
                eliminarFiscalEnCausa(c);
                break;
            default:
                printf("Operación cancelada.\n");
                break;
        }
    } else {
        printf("La causa no tiene fiscal asignado.\n");
        printf("¿Desea agregar uno? (1: Si, 0: No): ");
        scanf("%d", &opcion);
        getchar(); //WARNING

        if (opcion == 1) {
            printf("Ingrese nombre del fiscal: ");
            fgets(nombre, sizeof(nombre), stdin);
            nombre[strcspn(nombre, "\n")] = '\0';

            printf("Ingrese diligencias: ");
            fgets(diligencias, sizeof(diligencias), stdin);
            diligencias[strcspn(diligencias, "\n")] = '\0';

            c->fiscal = agregar_fiscal(nombre, diligencias, NULL); //ERROR POR PARAMETRO
            if (c->fiscal != NULL) {
                printf("Fiscal agregado exitosamente.\n");
            } else {
                printf("Error al agregar fiscal.\n");
            }
        } else {
            printf("Operación cancelada.\n");
        }
    }
}

void imprimir_Estadisticas(int abiertas, int cerradas, int archivadas, int enJuicio, int otras,int total) {
    printf("\n--- Estadísticas de causas ---\n");

    if (total == 0) {
        printf("No hay causas registradas.\n");
        return;
    }

    printf("Causas abiertas:   %d \n", abiertas);
    printf("Causas cerradas:   %d\n", cerradas);
    printf("Causas archivadas: %d\n", archivadas);
    printf("Causas en juicio:  %d\n", enJuicio);
    printf("Otras:             %d\n", otras);
}


void generar_Estadisticas(struct causa*lista,int total) {
    int abiertas = 0, cerradas = 0, archivadas = 0, enJuicio = 0, otras = 0,i;
    for (i = 0; i < total; i++) {
        if (strcmp(lista[i].estado_causa, "abierta") == 0) {
            abiertas++;
        } else if (strcmp(lista[i].estado_causa, "cerrada") == 0) {
            cerradas++;
        } else if (strcmp(lista[i].estado_causa, "archivada") == 0) {
            archivadas++;
        } else if (strcmp(lista[i].estado_causa, "en juicio") == 0) {
            enJuicio++;
        } else {
            otras++;
        }
    }
    imprimir_Estadisticas(abiertas,cerradas,archivadas,enJuicio,otras,total);
}

//FUNCIONES ESTADISTICAS
int contarCausas(struct causa*lista, int maxCausas) {
    int contador = 0,i;
    for (i = 0; i < maxCausas; i++) {
        if (lista[i].RUC  != 0) {
            contador ++;
        }
    }
    return contador;
}

void imprimirEstadisticas(int abiertas, int cerradas, int archivadas, int enJuicio, int otras,int total) {
    printf("\n--- Estadísticas de causas ---\n");

    if (total == 0) {
        printf("No hay causas registradas.\n");
        return;
    }

    printf("Causas abiertas:   %d \n", abiertas);
    printf("Causas cerradas:   %d\n", cerradas);
    printf("Causas archivadas: %d\n", archivadas);
    printf("Causas en juicio:  %d\n", enJuicio);
    printf("Otras:             %d\n", otras);
}


void generarEstadisticas(struct causa*lista,int total) {
    int abiertas = 0, cerradas = 0, archivadas = 0, enJuicio = 0, otras = 0,i;
    for (i = 0; i < total; i++) {
        if (strcmp(lista[i].estado_causa, "abierta") == 0) {
            abiertas++;
        } else if (strcmp(lista[i].estado_causa, "cerrada") == 0) {
            cerradas++;
        } else if (strcmp(lista[i].estado_causa, "archivada") == 0) {
            archivadas++;
        } else if (strcmp(lista[i].estado_causa, "en juicio") == 0) {
            enJuicio++;
        } else {
            otras++;
        }
    }
    imprimirEstadisticas(abiertas,cerradas,archivadas,enJuicio,otras,total);
}
void menuCausas(void) {
    int opcion, RUC, urgencia, maxCausas = 100;
    char estado[30];
    do {
        printf("\n==== MENU CAUSAS ====\n");
        printf("1. Agregar Causa\n");
        printf("2. Buscar Causa\n");
        printf("3. Modificar Causa\n");
        printf("4. Eliminar Causa\n");
        printf("5. Buscar Fiscal de una Causa\n");
        printf("6. Consultar Estadisticas de las Causas\n");
        printf("0. Volver\n");
        printf("Seleccione una opcion: ");
        scanf("%d", &opcion);
        getchar(); //WARNING
        switch(opcion) {
            case 1:
                printf("Ingrese RUC: \n");
                scanf("%d", &RUC);
                getchar(); //WARNING
                printf("Ingrese urgencia(vital [1],urgencia inmediata[2],urgencia[3],evidente[4],no urgente[5]) : \n");
                scanf("%d", &urgencia);
                getchar(); //WARNING
                printf("Ingrese estado de la causa (abierta,cerrada,archivada,en juicio,otra): \n");
                fgets(estado, sizeof(estado), stdin);
                estado[strcspn(estado, "\n")] = '\0';
                agregar_causa(&lista_causas, RUC, urgencia, estado);
                break;
            case 2:
                printf("Ingrese RUC a buscar: \n");
                scanf("%d", &RUC);
                getchar(); //WARNING
                buscar_causa(lista_causas, RUC);
                break;
            case 3:
                printf("Ingrese RUC de la causa a modificar: \n");
                scanf("%d", &RUC);
                getchar(); //WARNING
                printf("Ingrese nueva urgencia: \n");
                scanf("%d", &urgencia);
                getchar(); //WARNING
                printf("Ingrese nuevo estado: \n");
                fgets(estado, sizeof(estado), stdin);
                estado[strcspn(estado, "\n")] = '\0';
                modificar_causa(lista_causas, RUC, urgencia, estado);
                break;
            case 4:
                printf("Ingrese RUC de la causa a eliminar: \n");
                scanf("%d", &RUC);
                getchar(); //WARNING
                eliminar_causa(&lista_causas, RUC);
                break;
            case 5:
                buscarFiscalEnCausaMenu();
                break;
            case 6:
                generarEstadisticas(&lista_causas,maxCausas); //WARNING
                break;
            case 0:
                printf("Volviendo...\n");
                break;
            default:
                printf("Opcion invalida.\n");
                break;
        }
    } while(opcion != 0);
}
//MENU PRINCIPAL
int main() {
    int opcion;
    do {
        printf("\n==== FISCALIA REGIONAL DE VALPARAISO ====\n");
        printf("\nBienvenido a la Fiscalia Regional de Valparaiso\n");
        printf("Por favor seleccione la opcion que desea utilizar\n");
        printf("1. Denuncias\n");
        printf("2. Carpetas\n");
        printf("3. Imputados\n");
        printf("4. Causas\n");
        printf("0. Salir\n");
        printf("Seleccione una opcion: ");

        if (scanf("%d", &opcion) != 1) {
            printf("No se ingreso una opcion valida. Terminando el programa.\n");
            break;
        }
        getchar(); //WARNING

        switch(opcion) {
            case 1:
                menu_Denuncias();
                break;
            case 2:
                menuCarpetas();
                break;
            case 3:
                menuImputados();
                break;
            case 4:
                menuCausas();
                break;
            case 0:
                printf("Saliendo del sistema...\n");
                break;
            default:
                printf("Opcion invalida. Intente de nuevo.\n");
        }
    } while(opcion != 0);

    return 0;
}

   
