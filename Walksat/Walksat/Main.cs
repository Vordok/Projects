using System;
using System.Collections.Generic;
using System.Text;

namespace ConsoleApplication1
{
    class Program
    {
        static void Main(string[] args)
        {
            int m=3;
            int n=3;

            Walksat.walksat("(¬Dv¬Bv¬C)^(¬Bv¬Av¬C)^(¬Bv¬Cv¬E)^(¬Bv¬Cv¬E)", m, n);//los nombres de las variables no pueden contener 'v'
            if(Walksat.satisfacible())
                Console.Write("La sentencia es satisfacible.");
            else
                Console.Write("La sentencia no es satisfacible.");
            Console.ReadLine();
        }
    }
}
