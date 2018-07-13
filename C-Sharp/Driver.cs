using System;
using System.Diagnostics;

namespace C_Sharp
{
    class Driver
    {
        static void Main(string[] args)
        {
            //Simple s = new Simple(1000000);

            //s.RunSimple();

            //System.Diagnostics.PerformanceCounterCategory[] myCat2;
            //// Retrieve the categories.
            //myCat2 = System.Diagnostics.PerformanceCounterCategory.GetCategories();
            //foreach (var i in myCat2)
            //{
                
            //    if (i.CategoryName.StartsWith("Memo"))
            //    {
            //        Console.WriteLine(i.CategoryName);
            //        foreach (var j in i.GetCounters()) Console.WriteLine(j.CounterName);
            //        Console.WriteLine();
            //    }
                
            //}

            string selection;
            bool goodBit = false;
            Console.Write("C# Analysis Test Module\n1) Many small operations\n2) Complex mathematical calculations\n3) Large array manipulation\n4) Cryptographic hashing\n5) Recursive operation\n");


            while (!goodBit)
            {
                Console.Write("Your selection: ");
                selection = Console.ReadLine();

                try
                {
                    switch (selection)
                    {
                        case "1":
                            {
                                Console.Write("# Iterations? ");
                                int iterations;
                                selection = Console.ReadLine();
                                int.TryParse(selection, out iterations);
                                Simple s = new Simple(iterations);
                                s.RunSimple();
                                break;
                            }
                        case "2":
                            {
                                Console.Write("# Iterations? ");
                                int iterations;
                                selection = Console.ReadLine();
                                int.TryParse(selection, out iterations);
                                Complex co = new Complex(iterations);
                                co.RunComplex();
                                break;
                            }
                        case "3":
                            {
                                Console.Write("Array dimension size? ");
                                int size;
                                selection = Console.ReadLine();
                                int.TryParse(selection, out size);
                                Arrays a = new Arrays(size);
                                a.RunArrays();
                                break;
                            }
                        case "4":
                            {
                                Console.Write("# Iterations? ");
                                int iterations, size;
                                selection = Console.ReadLine();
                                int.TryParse(selection, out iterations);
                                Console.Write("Key size? ");
                                selection = Console.ReadLine();
                                int.TryParse(selection, out size);
                                Crypto c = new Crypto(iterations, size);
                                c.RunCrypto();
                                break;
                            }
                        case "5":
                            {
                                Console.Write("Recursive depth? ");
                                int depth;
                                selection = Console.ReadLine();
                                int.TryParse(selection, out depth);
                                Recursive r = new Recursive(depth);
                                r.RunRecursive();
                                break;
                            }
                    }
                    goodBit = true;
                }
                catch (System.Security.Cryptography.CryptographicException c)
                {
                    Console.Write(c.Message);
                }
                catch
                {
                    Console.Write("ERROR: Invalid selection. Type a number 1-5 to select a test module.\n");
                }
            }

        }
    }
}
