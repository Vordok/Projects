using System;
using System.Collections.Generic;
using System.Text;
using System.Collections;
using System.Threading;

namespace ConsoleApplication1
{
    class Walksat
    {
        static ArrayList clausulas;
        static ArrayList clausulasIn = new ArrayList();
        static int m, n, costoMinimo;
        static IDictionary<char,bool> combinacionMinCost = new Dictionary<char, bool>();
        static IDictionary<char, bool> valor = new Dictionary<char, bool>();
        static IDictionary<char, bool> valor2;
      
        public static void walksat(string sentencia,int m1,int n1) 
        {
            m=m1;
            n=n1;
            if (!sentencia.Equals("()"))
                getClausulas(sentencia);
        }

        static void getClausulas(string sentencia)
        {
            clausulas = new ArrayList();
            string nextToken = "";
            string[] st = sentencia.Split('^');
            foreach( string t in st )
            {
                for (int i = 1; i < t.Length - 1; i++)
                    nextToken = nextToken + t[i];
                clausulas.Add(nextToken);
                nextToken = "";
            }
            foreach (string clausula in clausulas)
                Console.WriteLine("{0}", clausula);
            Console.WriteLine();
        }

        static bool getRandomBool()
        {
            bool Res;
            Random num = new Random(DateTime.Now.Millisecond);
            int r = num.Next(10);

            if (r <=5 )
                Res = false;
            else
                Res = true;

            return (Res);
        }

        static void getVars()
        {
            foreach (string clausula in clausulas)
            {
                string[] st = clausula.Split('v');
                foreach (string t in st)
                {
                    if (t[0] != '¬')
                    {
                        if (valor.ContainsKey(t[0]) == false)
                        {
                            valor.Add(t[0], getRandomBool());
                        }
                    }
                    else
                    {
                        if (valor.ContainsKey(t[1]) == false)
                        {
                            valor.Add(t[1], getRandomBool());
                        }
                    }
                }
            }
        }

        static void getVars( string clausula )
        {
            valor2 = new Dictionary<char, bool>();

            string[] st = clausula.Split('v');
            foreach (string t in st)
            {
                if (t[0] != '¬')
                {
                    if (valor2.ContainsKey(t[0]) == false)
                    {
                        valor2.Add(t[0], valor[t[0]]);
                    }
                }
                else
                {
                    if (valor2.ContainsKey(t[1]) == false)
                    {
                        valor2.Add(t[1], valor[t[1]]);
                    }
                }
            }
        }   

        static IDictionary<char,bool> flip(char var)
        {
           IDictionary<char, bool> hm = new Dictionary<char, bool>(valor);
                   
            hm[var] = (hm[var])?false:true;

            Console.WriteLine("flip({0}):", var);
            foreach (KeyValuePair<char, bool> var1 in hm)
                Console.WriteLine("{0} = {1}", var1.Key, var1.Value);
            Console.WriteLine();

            return hm;
        }

        static int getCosto(IDictionary<char, bool> valor)
        {
            int costo=0;
            foreach(string clausula in clausulas)
                costo=costo+(clausulaVerdadera(clausula,valor)?0:1);
            return costo;
        }
        
        static int getCombMinCost( string clausulaRandom )
        {
            int costo, i=0;
            costoMinimo=-1;
            Random num = new Random(DateTime.Now.Millisecond);
            int p = num.Next(2);
            
            IDictionary<char, bool> flippedComb;
            char[] keys = new char[valor.Count];

            getVars(clausulaRandom);

            int r = num.Next(valor2.Count);

            foreach (KeyValuePair<char, bool> var in valor2)
            {
                keys[i] = var.Key;
                i++;
            }

            if (p == 0)
            {
                flippedComb = flip(keys[r]);
                costo = getCosto(flippedComb);
                return costo;
            }
            else if (p == 1)
            {
                foreach (char var in keys)
                {
                    flippedComb = flip(var);
                    costo = getCosto(flippedComb);
                    if (costoMinimo == -1 || costo < costoMinimo)
                    {
                        costoMinimo = costo;
                        combinacionMinCost = flippedComb;
                        return costoMinimo;
                    }
                }
            }
            return -1;
        }
        
        public static bool satisfacible()
        {
            bool satisf=false;
            Random num = new Random(DateTime.Now.Millisecond);
        
            if(clausulas==null)
                return satisf;

            for(int i=0;i<n;i++)
            {
                Console.WriteLine("n= {0} ***************************************************************", i);
                getVars();
                
                for(int j=0;j<m;j++)
                {
                    Console.WriteLine("m= {0} ----------------------------------", j);
                    Console.WriteLine();
                    foreach(KeyValuePair<char, bool> var in valor)
                        Console.WriteLine("{0} = {1}",var.Key, var.Value);
                    Console.WriteLine();
                    int costoTemp=getCosto(valor);
                    Console.WriteLine("costo: {0}",costoTemp);
                    Console.WriteLine();
                    if (costoTemp == 0)
                        return true;
                    if (clausulasIn == null)
                    {
                        satisf = true;
                        return satisf;
                    }
                    int r = num.Next(clausulasIn.Count);
                    int costoMinimo = getCombMinCost(clausulasIn[r].ToString());
                    Console.WriteLine("Costo minimo actual: {0}", costoMinimo);
                    Console.WriteLine();
                    if (costoMinimo == 0)
                        return true;
                    valor=combinacionMinCost;
                    clausulasIn.Clear();

                }
            }
            return satisf;
        }    
        
        static bool clausulaVerdadera(string clausula, IDictionary<char, bool> valor)
        {
            bool val, val1 = false;
            IDictionary<char, bool> valor1 = new Dictionary<char, bool>();
            

            for (int i = 0; i < clausula.Length; i++)
            {
                if (clausula[i] == '¬')
                {
                    val = valor[clausula[i + 1]];
                    if (val == false)
                        valor1.Add(clausula[i + 1], true);
                    else if (val == true)
                        valor1.Add(clausula[i + 1], false);
                    i++;
                }
                else if (clausula[i] == 'v')
                    continue;
                else
                    valor1.Add(clausula[i], valor[clausula[i]]);
            }

            foreach (KeyValuePair<char, bool> kvp in valor1)
            {
                if (kvp.Value == true)
                {
                    val1 = true;
                    break;
                }
            }
            if (val1 == false)
                clausulasIn.Add(clausula);

            return val1;
        }
    }
}
