using System;

class Program
{
    static readonly byte[] Enc = { 0x3F, 0x29, 0x28, 0x3F, 0x2C, 0x3F, 0x28, 0x7B };

    static bool Check(string s)
    {
        if (s.Length != Enc.Length) return false;
        for (int i = 0; i < Enc.Length; i++)
            if ((s[i] ^ 0x5A) != Enc[i]) return false;
        return true;
    }

    static int Main(string[] args)
    {
        if (args.Length > 0 && Check(args[0])) { Console.WriteLine("Correct!"); return 0; }
        Console.WriteLine("Wrong.");
        return 1;
    }
}
