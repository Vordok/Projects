using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace Search_Algorithms
{
    public partial class Form3 : Form
    {
        public Form3()
        {
            InitializeComponent();
        }

        private void button1_Click(object sender, EventArgs e)//metodo para el boton regresar
        {
            this.DialogResult = DialogResult.OK;
            Close();//se cierra la ventana
        }
    }
}