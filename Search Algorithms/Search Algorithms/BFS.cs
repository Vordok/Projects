using System;

namespace Search_Algorithms
{
    class BFS
    {
        static int r = 0, ru = 0, corr = 0, j = 0, k = 0;//variables que se usaran como banderas o como indices
        static int[] ruta = new int[20];//arreglo donde se guardara la ruta usando los identificadores
        static int[] hijos = new int[300];//arreglo donde s eguardaran los hijos del nodo actual
        static int[] expan = new int[20];
        static string[] rut = new string[20];//arreglo donde se guardara la ruta utilizando los nombres de las ciudades

        public static string[] Busqueda(int inicio, int destino, int indice)//En este metodo se realiza la busqueda
        {
            string[] ciudad1 = new string[indice];//en este arreglo se guardaran los nombres de las ciduades
            bool exp = false;//esta variable sirve como bandera para saber si ya se expandio un nodo
            bool rutea = false;

            for (int i = 0; i < 20; i++)//todos los arreglos enteros se inicializaran en -1
            {
                if (corr == 0)//en caso de que sea la primera vez que s eingreza al metodo hace el llenado
                {
                    for (int m = 0; m < 300; m++)
                        hijos[m] = -1;
                    ruta[i] = -1;
                    expan[i] = -1;
                }
                else
                    i = 20;
            }

            if (inicio != destino)//se verifica que no se haya llegado ya al destino
            {
                int[] conexion1 = new int[indice];//arreglo para guardar la sconexiones del nodo seleccionado
                bool dest = false;

                conexion1 = Archivo.Conexiones(inicio);//se invoca el metodo conexiones para recibir las conexiones del nodo
                int c = conexion1.Length;//contador

                expan[k] = inicio;
                k++;

                for (int i = 0; i < c; i++)//captura de hijos
                {
                    if (conexion1[i] > 0)//en caso que en la posicion actual el valor sea mayor a cero se toma como hijo y se llena el arreglo
                    {
                        //se manjea como cola el primero que entra es el primero en salir
                        hijos[j + 1] = hijos[j];
                        hijos[j] = i;
                        if (hijos[j] == destino)
                        {
                            ruta = expan;
                            dest = true;
                            break;
                        }
                        j++;
                    }
                }
                if (dest == true)
                {
                    ciudad1 = Archivo.Ciudades();//se invoca el metodo ciudades para obtener los nombres de las mismas
                    Ruta(ruta, indice, rut, ciudad1);//se invoca el metodo ruta para obtener la ruta con nombres
                    return rut;
                }
      
                if(corr == 0)
                {
                    ruta[r] = inicio;//se coloca dentro de la ruta el nodo con el que se inicios
                    r++;//se incrementa el contador de nodos en la ruta
                }
                for (int i = 0; i < j; i++)//ciclo para ver cual es el siguiente nodo inicial
                {
                    for (int m = 0; m < k; m++)//ciclo que sirve apara verificar si el nodo ya fue expandido
                    {
                        if (hijos[i] == expan[m])
                        {
                            m = k;
                            exp = true;
                        }
                        else if (hijos[i] >= hijos[i + 1] && hijos[i + 1] > 0 && rutea == false)
                        {
                            int cont = 0;
                            for (int l = 0; l < r; l++)
                            {
                                if (ruta[l] != hijos[r-1])
                                {
                                    cont++;
                                }
                            }
                            if (cont == r)
                            {
                                ruta[r] = hijos[r - 1];//se coloca dentro de la ruta el nodo con el que se inicio
                                r++;//se incrementa el contador de nodos en la ruta
                                rutea = true;
                            }
                        }
                    }
                    if (exp == false)//en caso de no haber sido expandido ese nodo se toma como el nuevo nodo inicial
                    {
                        inicio = hijos[i];
                        i = j;
                    }
                    exp = false;
                }
                corr++;//indica que ya se invoco el metodopor lo menos una vez
                Busqueda(inicio, destino, indice);//se invoca nuevamente la funcion para seguir la busqueda
            }
            else//en caso de que se haya llegado al destino
            {
                ciudad1 = Archivo.Ciudades();//se invoca el metodo ciudades para obtener los nombres de las mismas
                Ruta(ruta, indice, rut, ciudad1);//se invoca el metodo ruta para obtener la ruta con nombres
            }
            return rut;
        }
        private static void Ruta(int[] ruta, int indice, string[] rut, string[] ciudad1)
        {
            r = ruta.Length;
            for (int i = 0; i < indice; i++)
            {
                if (i == ruta[ru] && ru < r)//si el identificador de la ciudad se encuentra dentro del arreglo de ruta entero se guarda en el arreglo de cadenas
                {
                    rut[ru] = ciudad1[i];
                    ru++;
                    Ruta(ruta, indice, rut, ciudad1);//se vuelve a invocar la funcion ya que el dientificador puede estar antes del que se acaba de pasar
                }
                else if (ru == r)
                    i = indice;
            }
        }
    }
}
