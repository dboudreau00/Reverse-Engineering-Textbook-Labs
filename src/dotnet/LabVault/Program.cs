// LabVault: keeps one secret in vault.json, protected by a password.
using System;
using System.IO;
using System.Security.Cryptography;
using System.Text;
using System.Text.Json;

record Vault(string Salt, int Iterations, string Nonce, string Ciphertext, string Tag);

static class Program
{
    static byte[] DeriveKey(string password, byte[] salt, int iterations) =>
        Rfc2898DeriveBytes.Pbkdf2(Encoding.UTF8.GetBytes(password), salt, iterations,
                                  HashAlgorithmName.SHA256, 32);

    static int Main(string[] args)
    {
        if (args.Length >= 3 && args[0] == "lock")
        {
            int iterations = args.Length == 4 ? int.Parse(args[3]) : 600_000;
            byte[] salt = RandomNumberGenerator.GetBytes(16);
            byte[] nonce = RandomNumberGenerator.GetBytes(12);
            byte[] plain = Encoding.UTF8.GetBytes(args[2]);
            byte[] cipher = new byte[plain.Length];
            byte[] tag = new byte[16];
            using var gcm = new AesGcm(DeriveKey(args[1], salt, iterations), 16);
            gcm.Encrypt(nonce, plain, cipher, tag);
            var vault = new Vault(Convert.ToBase64String(salt), iterations, Convert.ToBase64String(nonce),
                                  Convert.ToBase64String(cipher), Convert.ToBase64String(tag));
            File.WriteAllText("vault.json", JsonSerializer.Serialize(vault));
            Console.WriteLine("vault.json written");
            return 0;
        }
        if (args.Length == 2 && args[0] == "open")
        {
            var v = JsonSerializer.Deserialize<Vault>(File.ReadAllText("vault.json"))!;
            byte[] cipher = Convert.FromBase64String(v.Ciphertext);
            byte[] plain = new byte[cipher.Length];
            using var gcm = new AesGcm(DeriveKey(args[1], Convert.FromBase64String(v.Salt), v.Iterations), 16);
            try
            {
                gcm.Decrypt(Convert.FromBase64String(v.Nonce), cipher, Convert.FromBase64String(v.Tag), plain);
            }
            catch (AuthenticationTagMismatchException)
            {
                Console.WriteLine("Wrong password.");
                return 1;
            }
            Console.WriteLine(Encoding.UTF8.GetString(plain));
            return 0;
        }
        Console.WriteLine("usage: LabVault lock <password> <secret> [iterations] | LabVault open <password>");
        return 2;
    }
}
