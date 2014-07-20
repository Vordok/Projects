using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace Search_Algorithms
{
    public partial class Form1 : Form
    {
        int inicio, destino, metodo, j;
        string[] ruta;
        string[] ruta1;

        public Form1()
        {
            string[] ciudad;//arreglo donde se guardaran los nombres de las ciudades

            ciudad = Archivo.Ciudades();//se invoca al metodo ciudades de la clase archivo y se gaurda el resultado
            InitializeComponent();//se inicializan los objetos de la ventana

            for (int i = 0; i < ciudad.Length; i++)
            {
                comboBox1.Items.Add(ciudad[i]);//se llena el combo box con las ciudades a escoger
                comboBox2.Items.Add(ciudad[i]);
                j = i;
            }
            comboBox3.Items.Add("DFS");//se llena el ultimo combo box con las posibilidades de metodos
            comboBox3.Items.Add("BFS");
        }

        private void button2_Click(object sender, EventArgs e)//metodo para el boton de ejecutar
        {
            //en las siguientes tres lineas se toman las elecciones en los combo boxes
            inicio = comboBox1.SelectedIndex;
            destino = comboBox2.SelectedIndex;
            metodo = comboBox3.SelectedIndex;
            if (inicio == destino)//se verifica que el inicio y le destino no sean iguales
            {
                //en caso de que si lo sean aparece una nueva ventana señalando el error
                Form2 aviso = new Form2();
                this.DialogResult = DialogResult.OK;
                aviso.Show();
            }else if( inicio == -1 || destino == -1 || metodo == -1 )//se verifica que ningun combo box este vacio
            {
                //en caso de que si este vacio alguno aparece una nueva ventana indicando el error
                Form3 aviso1 = new Form3();
                this.DialogResult = DialogResult.OK;
                aviso1.Show();
            }

            Form4 Nvaventana = new Form4();//se crea un objeto para la siguiente ventana

            switch (metodo)//se ve que metodo fue seleccionado
            {
                case 0://si fue el metodo 0 se invoca a la clase DFS
                    ruta = DFS.Busqueda(inicio, destino, j);//se invoca al metodo busqueda de la clase DFS y se regresa la ruta encontrada
                    int r = 0;//variable que nos ayudara a saber cuantos nodos conforman la ruta
                    Hide();//Cierra la ventana actual
                    Nvaventana.Show();//Se muestra la siguiente ventana
                    for (int i = 0; i < ruta.Length; i++)//en este ciclo sabremos cuantos nodos hay en la ruta
                    {
                        if (ruta[i] != null)//si el nombre es diferente a nulo se aumenta en uno a r
                            r++;
                    }
                    for (int i = 0; i < r; i++)//en este ciclo se imprime la ruta
                    {
                        Nvaventana.textBox1.AppendText( ruta[i]);//se imprime uno a uno los nodos de la ruta
                        if( i != r - 1 && ruta.Length > 1 )
                            Nvaventana.textBox1.AppendText(" - ");
                    }
                    break;

                case 1://si fue el metodo 1 se invoca a la clase BFS
                    ruta1 = BFS.Busqueda(inicio, destino, j);//se invoca al metodo busqueda de la clase DBFS y se regresa la ruta encontrada
                    int r1 = 0;//variable que nos ayudara a saber cuantos nodos conforman la ruta
                    Hide();//Cierra la ventana actual
                    Nvaventana.Show();//Se muestra la siguiente ventana
                    for (int i = 0; i < ruta1.Length; i++)//en este ciclo sabremos cuantos nodos hay en la ruta
                    {
                        if (ruta1[i] != null)//si el nombre es diferente a nulo se aumenta en uno a r
                            r1++;
                    }
                    for (int i = 0; i < r1; i++)//en este ciclo se imprime la ruta
                    {
                        Nvaventana.textBox1.AppendText(ruta1[i]);//se imprime uno a uno los nodos de la ruta
                        if (i != r1 - 1 && ruta1.Length > 1)
                            Nvaventana.textBox1.AppendText(" - ");
                    }
                    break;

                default:
                    break;
            }
        }
    }
}