using System;
using System.IO;

/* En esta clase se realiza la apertura y lectura de los archivos necesarios*/
 
namespace Search_Algorithms
{
    public class Archivo
    {
        static int indice = 0;
        public static int[] Conexiones( int nodo )//En este metodo se lee el archivo donde se encuntran las conexiones del nodo elegido y se guardan en un arreglo
        {
            char[] sep=new char[]{'|'};//arreglo que nos ayudara a identificar el separador de las conexiones
            int n = -1;//variable para comparar si es el nodo que buscamos
            string linea;//variable donde se guarda la linea leida
            string[] campos;//variable para guardar los campos del archivo y despues pasarlos a conexion

            FileStream Archivo = new FileStream( "conexiones.txt", FileMode.Open, FileAccess.Read );//Abre el archivo donde vienen las conexiones
            StreamReader master = new StreamReader( Archivo );//abre el flujo de lectura del archivo

            int[] conexion = new int[indice];//arreglo donde se guardaran las conexiones

            while ( n != nodo )//lee el archivo hasta llegar al final del mismo
            {
                linea = master.ReadLine();//se lee la linea completa
                campos = linea.Split(sep);//se guarda la linea sin los separadores en un arreglo
                n = Int32.Parse(campos[0]);
                if (n == nodo)//compara el dato obtenido con el nodo solicitado
                {
                    for (int j = 0; j < indice; j++)//ciclo para pasar los datos de campos a conexion
                    {
                        conexion[ j ] = Int32.Parse( campos[ j + 1 ] );//se realiza el llenado de conexion
                    }
                }
            }

            master.Close();//cierra el flujo de lectura
            Archivo.Close();//cierra al archivo

            return conexion;//regresa el arreglo de conexion
        }

        public static string[] Ciudades()//En este metodo se lee el archivo de indices
        {
            char[] sep = new char[] { '|' };//arreglo que nos ayudara a identificar el separador de las conexiones
            string linea;//variable donde se guarda la linea leida
            string[] campos = new string [ 2 ];//variable para guardar los campos del archivo y despues pasarlos a conexion
            int i = 0;

            FileStream Archivo1 = new FileStream("indices.txt", FileMode.Open, FileAccess.Read);//Abre el archivo donde vienen las conexiones
            StreamReader master1 = new StreamReader(Archivo1);//abre el flujo de lectura del archivo

            string[] ciudades = new string[20];//arreglo donde se guardaran las ciudades

            while ( master1.Peek() != -1 )//lee el archivo hasta llegar al final del mismo
            {
                linea = master1.ReadLine();//se lee la linea completa
                campos = linea.Split(sep);//se guarda la linea sin los separadores en un arreglo
                ciudades[ i ] = campos[ 0 ];//se realiza el llenado de las ciudades
                i++;
            }

            indice = i;

            if (i < 20)
            {
                string[] ciudadesaux = new string[i];
                for (int j = 0; j < i; j++)
                {
                    ciudadesaux[j] = ciudades[j];
                }
                ciudades = null;
                return ciudadesaux;
            }
            
            master1.Close();//cierra el flujo de lectura
            Archivo1.Close();//cierra al archivo

            return ciudades;//regresa el arreglo de ciudades
        }
    }
}
