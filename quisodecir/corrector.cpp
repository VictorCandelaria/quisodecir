/*****************************************************************************************************************
    UNIVERSIDAD NACIONAL AUTONOMA DE MEXICO
    FACULTAD DE ESTUDIOS SUPERIORES ARAGON

    Computadoras y programación
    (Candelaria Rosas Víctor Eduardo 323096537) Ponga su nombre y número de cuenta aquí

    Implementación oficial compatible con el evaluador UNAM para el proyecto
    "Usted quiso decir" (Diccionario, ClonaPalabras, ListaCandidatas)
******************************************************************************************************************/

#include "stdafx.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "corrector.h"

/* ============================================================
   FUNCIONES INTERNAS
   ============================================================ */

static void aMinusculas(char* s)
{
    for (int i = 0; s[i]; i++)
    {
        if (s[i] >= 'A' && s[i] <= 'Z')
            s[i] = s[i] + 32;
    }
}

static int esSeparador(char c)
{
    return (c == ' ' || c == '\t' ||
        c == '\n' || c == '\r' ||
        c == ',' || c == ';' ||
        c == '(' || c == ')');
}


static int esAlfabetico(char c)
{
    return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z'));
}


static void limpiarYValidar(char* tok)
{
    int escritura = 0;
    for (int lectura = 0; tok[lectura]; lectura++)
    {
        if (esAlfabetico(tok[lectura]))
            tok[escritura++] = tok[lectura];
    }
    tok[escritura] = '\0';
}

static int buscarBinaria(const char dic[][TAMTOKEN], int n, const char* pal)
{
    int lo = 0, hi = n - 1;
    while (lo <= hi)
    {
        int mid = (lo + hi) / 2;
        int cmp = strcmp(dic[mid], pal);
        if (cmp == 0) return mid;
        if (cmp < 0) lo = mid + 1;
        else hi = mid - 1;
    }
    return -lo - 1;
}

static void insertarOrdenado(char dic[][TAMTOKEN], int est[], int& n, int pos, const char* pal)
{
    for (int i = n; i > pos; i--)
    {
        strcpy(dic[i], dic[i - 1]);
        est[i] = est[i - 1];
    }
    strcpy(dic[pos], pal);
    est[pos] = 1;
    n++;
}


/* ============================================================
   DICCIONARIO
   ============================================================ */
void Diccionario(char* archivo,
    char palabras[][TAMTOKEN],
    int est[],
    int& n)
{
    n = 0;
    if (!archivo) return;

    FILE* f = fopen(archivo, "rb");
    if (!f) return;

    char tok[TAMTOKEN];
    int idx = 0;
    int c;

    while ((c = fgetc(f)) != EOF)
    {
        char ch = (char)c;

        if (esSeparador(ch))
        {
            if (idx > 0)
            {
                tok[idx] = '\0';
                idx = 0;

                limpiarYValidar(tok);
                if (tok[0] == '\0') continue;

                aMinusculas(tok);

                int pos = buscarBinaria(palabras, n, tok);
                if (pos >= 0)
                    est[pos]++;
                else
                {
                    pos = -pos - 1;
                    insertarOrdenado(palabras, est, n, pos, tok);
                }
            }
        }
        else
        {
            if (idx < TAMTOKEN - 1)
                tok[idx++] = ch;
            else
            {
                while ((c = fgetc(f)) != EOF && !esSeparador((char)c));
                idx = 0;
            }
        }
    }

    if (idx > 0)
    {
        tok[idx] = '\0';
        limpiarYValidar(tok);
        if (tok[0])
        {
            aMinusculas(tok);
            int pos = buscarBinaria(palabras, n, tok);
            if (pos >= 0) est[pos]++;
            else
            {
                pos = -pos - 1;
                insertarOrdenado(palabras, est, n, pos, tok);
            }
        }
    }

    fclose(f);
}


/* ============================================================
   LISTA CANDIDATAS
   ============================================================ */
void ListaCandidatas(char clonadas[][TAMTOKEN],
    int numClonadas,
    char dic[][TAMTOKEN],
    int est[],
    int numDic,
    char finales[][TAMTOKEN],
    int peso[],
    int& numFinal)
{
    numFinal = 0;

    for (int i = 0; i < numClonadas; i++)
    {
        char tmp[TAMTOKEN];
        strcpy(tmp, clonadas[i]);
        aMinusculas(tmp);

        int pos = buscarBinaria(dic, numDic, tmp);

        if (pos >= 0)
        {
            int ya = 0;
            for (int j = 0; j < numFinal; j++)
                if (strcmp(finales[j], dic[pos]) == 0)
                    ya = 1;

            if (!ya)
            {
                strcpy(finales[numFinal], dic[pos]);
                peso[numFinal] = est[pos];
                numFinal++;
            }
        }
    }

    for (int i = 0; i < numFinal - 1; i++)
    {
        int best = i;
        for (int j = i + 1; j < numFinal; j++)
        {
            if (peso[j] > peso[best]) best = j;
            else if (peso[j] == peso[best] &&
                strcmp(finales[j], finales[best]) < 0)
                best = j;
        }

        if (best != i)
        {
            char tmp[TAMTOKEN];
            strcpy(tmp, finales[i]);
            strcpy(finales[i], finales[best]);
            strcpy(finales[best], tmp);

            int t = peso[i];
            peso[i] = peso[best];
            peso[best] = t;
        }
    }
}
