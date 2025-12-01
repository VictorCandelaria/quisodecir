/*****************************************************************************************************************
	UNIVERSIDAD NACIONAL AUTONOMA DE MEXICO
	FACULTAD DE ESTUDIOS SUPERIORES -ARAGON-

	Computadoras y programacion. 
	(c) Ernesto Peñaloza Romero 2015
	
	Quiso decir: Programa principal de la aplicacion de la distancia de Levenstein.
	
******************************************************************************************************************/

#include "stdafx.h"
#include <string.h>
#include "corrector.h"

int _tmain(int argc, _TCHAR* argv[])
{
	char	szNombre[50];
	char	szPalabras[NUMPALABRAS][TAMTOKEN];
	int		iEstadisticas[NUMPALABRAS];
	int		i;
	int		iNumElementos;
	int		iNumSugeridas;	
	char	szPalabraLeida[TAMTOKEN];
	char	szPalabrasSugeridas[3300][TAMTOKEN];
	char	szListaFinal[3300][TAMTOKEN];
	int		iPeso[3300]; //son 66 combinaciones por letra, mas o menos 3300 para 50 letras
	int		iNumLista;

	//Inicia el arreglo de estadisticas
	for (i=0; i< NUMPALABRAS; i++)
		iEstadisticas[i] = 0;

	//Lee el nombre del archivo desde donde se recupera el diccionario
	//strcpy(szNombre, "anita.txt"); //depurar con este archivo, es el de la especificacion
	printf("Archivo para el diccionario: ");
	scanf("%s",szNombre);

	//carga el diccionario
	Diccionario(szNombre, szPalabras, iEstadisticas, iNumElementos);

	//Pinta las palabras del diccionario y sus estadisticas
	for (int j=0; j < iNumElementos-1; j++)
		printf("%-12s %i\n", szPalabras[j],iEstadisticas[j]);
		
	//Mientras no se capture el comando para salir y poner <fin>
	strcpy(szPalabraLeida, "");
	while (strcmp(szPalabraLeida, "fin"))
	{
		//pide una palabra
		printf("Palabra---'fin' para terminar---: ");
		scanf("%s", szPalabraLeida);
		_strlwr(szPalabraLeida);

		//calcula las palabras del algortimo
		ClonaPalabras(szPalabraLeida, szPalabrasSugeridas, iNumSugeridas);


		//Pinta la lista de palabras calculadas
		for (int j=0; j < iNumSugeridas-1; j++)
			printf("%s\n", szPalabrasSugeridas[j]);

		//Con esa lista de palabras y el diccionario, recupera las sugerencias
		ListaCandidatas(szPalabrasSugeridas, iNumSugeridas, szPalabras, iEstadisticas, iNumElementos,
						szListaFinal, iPeso, iNumLista);

		//Mostrar las opciones candidatas, por orden de peso(frecuencia)
		printf("Las candidatas son \n");
		for (int j=0; j < iNumLista; j++)
			printf("%s %i\n", szListaFinal[j], iPeso[j]);
	}

	return 0;
}

