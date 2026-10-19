// Lab 9b: a harmless program that only prints its own encrypted configuration.
using System;
using System.Security.Cryptography;
using System.Text;

static class Settings
{
    public static string Key   = "4ZZVPHayvzfBiRYqtqwZ4W2SrjQFj7qDtntMg7WtoCE=";
    public static string Host  = "X4vq8v5kSX+YEwuVihbkOm1V7DjcgzB5Fbwp9HROaPFsARJ5D5U34w4kKAAUesVvq1w0zThrr28dsXFDhTZvXA==";
    public static string Port  = "PQ52TTXi9pd3rOag/ywcnEQ9RjgdEeC0Y7t83brnaDsB6OngYqkq23KiHz1A+fxOQv1/yookqStjIZzeWhNKzw==";
    public static string Mutex = "Z44mFP6ozVhnPSh9UouNOa6EWjy8A+N6Gnds47Fz61o+jCiXZPn/x3eCDOg6koEIKmUDPfsVDnnZP6jCDEM10w==";
    public static string Group = "DBAPAz5X0gomnI87Ps5KLZ3c2S4orjrqvr3TyGTxLuh5l+DFEOzaZw5KC0ri3hVhMsnbNqD7aizvIVOgwVD3iQ==";
}

static class Crypto
{
    static readonly byte[] Salt = Encoding.ASCII.GetBytes("esreveR-lab-salt");

    public static string Decrypt(string b64, byte[] masterKey)
    {
        byte[] blob = Convert.FromBase64String(b64);
        byte[] keys = Rfc2898DeriveBytes.Pbkdf2(masterKey, Salt, 50000, HashAlgorithmName.SHA256, 64);
        byte[] aesKey = keys[..32], macKey = keys[32..];
        byte[] mac = blob[..32], iv = blob[32..48], ct = blob[48..];

        using var hmac = new HMACSHA256(macKey);
        if (!CryptographicOperations.FixedTimeEquals(mac, hmac.ComputeHash(blob[32..])))
            throw new CryptographicException("bad MAC");

        using var aes = Aes.Create();
        aes.Key = aesKey;
        return Encoding.UTF8.GetString(aes.DecryptCbc(ct, iv));
    }
}

class Program
{
    static void Main()
    {
        byte[] master = Convert.FromBase64String(Settings.Key);
        Console.WriteLine("Host  = " + Crypto.Decrypt(Settings.Host, master));
        Console.WriteLine("Port  = " + Crypto.Decrypt(Settings.Port, master));
        Console.WriteLine("Mutex = " + Crypto.Decrypt(Settings.Mutex, master));
        Console.WriteLine("Group = " + Crypto.Decrypt(Settings.Group, master));
    }
}
