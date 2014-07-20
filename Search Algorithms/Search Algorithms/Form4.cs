using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace Search_Algorithms
{
    public partial class Form4 : Form
    {
        public Form4()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)//metodo para el boton de finalizar
        {
            Application.ExitThread();//al ser presionado el boton el programa termina
        }

        private void button2_Click(object sender, EventArgs e)
        {
            Close();
            Application.Restart();
        }
    }
}