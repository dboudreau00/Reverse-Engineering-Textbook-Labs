// Lab 14.3: Lab 7's check, obfuscated by hand the way protectors do it automatically.
// Lookalike Unicode names, strings decrypted by a module initializer, a switch-driven loop.
using System;
using System.Runtime.CompilerServices;

static class ǀ
{
    static readonly byte[] ǀǀ = { 0x3F, 0x29, 0x28, 0x3F, 0x2C, 0x3F, 0x28, 0x7B };
    static readonly byte[] ǀǀǀ = { 0x19, 0x35, 0x28, 0x28, 0x3F, 0x39, 0x2E, 0x7B };   // "Correct!" ^ 0x5A
    static readonly byte[] ǀǀǀǀ = { 0x0D, 0x28, 0x35, 0x34, 0x3D, 0x74 };              // "Wrong."   ^ 0x5A
    internal static string Ⅰ, Ⅱ;

    [ModuleInitializer]                       // runs before Main: <Module>..cctor calls it
    internal static void Ⅲ()
    {
        Ⅰ = Ⅳ(ǀǀǀ);
        Ⅱ = Ⅳ(ǀǀǀǀ);
    }

    static string Ⅳ(byte[] b)
    {
        var c = new char[b.Length];
        for (int i = 0; i < b.Length; i++) c[i] = (char)(b[i] ^ 0x5A);
        return new string(c);
    }

    internal static bool Ⅴ(string s)
    {
        int state = 0, i = 0;
        while (true)
        {
            switch (state)
            {
                case 0: state = s.Length == ǀǀ.Length ? 1 : 4; break;
                case 1: state = i < ǀǀ.Length ? 2 : 3; break;
                case 2: state = (s[i] ^ 0x5A) == ǀǀ[i] ? 5 : 4; break;
                case 5: i++; state = 1; break;
                case 3: return true;
                default: return false;
            }
        }
    }

    static int Main(string[] args)
    {
        if (args.Length > 0 && Ⅴ(args[0])) { Console.WriteLine(Ⅰ); return 0; }
        Console.WriteLine(Ⅱ);
        return 1;
    }
}
