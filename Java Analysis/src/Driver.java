import java.util.Scanner;

public class Driver {

	public static void main(String[] args) {
		Scanner input = new Scanner(System.in);
		
		int selection;
		boolean goodBit = false;
        System.out.print("Java Analysis Test Module\n"
        		+ "1) Many small operations\n"
        		+ "2) Complex mathematical calculations\n"
        		+ "3) Large array manipulation\n"
        		+ "4) Cryptographic hashing\n"
        		+ "5) Recursive operation\n");

        while (!goodBit)
        {
        	System.out.print("Your selection: ");
            selection = input.nextInt();

            try
            {
                switch (selection)
                {
                    case 1:
                        {
                        	System.out.print("# Iterations? ");
                            int iterations;
                            iterations = input.nextInt();
                            Simple s = new Simple(iterations);
                            s.RunSimple();
                            break;
                        }
                    case 2:
                        {
                        	System.out.print("# Iterations? ");
                            int iterations;
                            iterations = input.nextInt();
                            Complex co = new Complex(iterations);
                            co.RunComplex();
                            break;
                        }
                    case 3:
                        {
                        	System.out.print("Array dimension size? ");
                            int size;
                            size = input.nextInt();
                            Arrays a = new Arrays(size);
                            a.RunArrays();
                            break;
                        }
                    case 4:
                        {
                        	System.out.print("# Iterations? ");
                            int iterations, size;
                            iterations = input.nextInt();
                            System.out.print("Key size? ");
                            size = input.nextInt();
                            Crypto c = new Crypto(iterations, size);
                            c.RunCrypto();
                            break;
                        }
                    case 5:
                        {
                        	System.out.print("Recursive depth? ");
                            int depth;
                            depth = input.nextInt();
                            Recursive r = new Recursive(depth);
                            r.RunRecursive();
                            break;
                        }
                }
                goodBit = true;
            }
            catch (Exception e)
            {
            	System.out.print("ERROR: Invalid selection. Type a number 1-5 to select a test module.\n");
            }
        }
        input.close();
	}

}
