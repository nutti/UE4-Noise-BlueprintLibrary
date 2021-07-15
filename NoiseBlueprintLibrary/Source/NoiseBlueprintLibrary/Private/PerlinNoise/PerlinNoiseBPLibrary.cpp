#include "PerlinNoise/PerlinNoiseBPLibrary.h"

namespace
{
    float Grad(int hash, float x, float y, float z)
    {
        switch (hash & 0x0F)
        {
            case 0x00:
                return x + y;
            case 0x01:
                return -x + y;
            case 0x02:
                return x - y;
            case 0x03:
                return -x - y;
            case 0x04:
                return x + z;
            case 0x05:
                return -x + z;
            case 0x06:
                return x - z;
            case 0x07:
                return -x - z;
            case 0x08:
                return y + z;
            case 0x09:
                return -y + z;
            case 0x0A:
                return y - z;
            case 0x0B:
                return -y - z;
            case 0x0C:
                return y + x;
            case 0x0D:
                return -y + z;
            case 0x0E:
                return y - x;
            case 0x0F:
                return -y - z;
        }

        // must not reach here.
        return 0.0f;
    }

    float Lerp(float A, float B, float T) noexcept
    {
        return A + (B - A) * T;
    }

    float Fade(float T) noexcept
    {
        // 6T^5 - 15T^4 + 10T^3
        return T * T * T * (T * (T * 6.0 - 15.0) + 10.0);
    }

    int* HashTbl;

    static int HashTblBase[] =
    {
        151, 160, 137, 91, 90, 15,
        131, 13, 201, 95, 96, 53, 194, 233, 7, 225, 140, 36, 103, 30, 69, 142, 8, 99, 37, 240, 21, 10, 23,
        190, 6, 148, 247, 120, 234, 75, 0, 26, 197, 62, 94, 252, 219, 203, 117, 35, 11, 32, 57, 177, 33,
        88, 237, 149, 56, 87, 174, 20, 125, 136, 171, 168, 68, 175, 74, 165, 71, 134, 139, 48, 27, 166,
        77, 146, 158, 231, 83, 111, 229, 122, 60, 211, 133, 230, 220, 105, 92, 41, 55, 46, 245, 40, 244,
        102, 143, 54, 65, 25, 63, 161, 1, 216, 80, 73, 209, 76, 132, 187, 208, 89, 18, 169, 200, 196,
        135, 130, 116, 188, 159, 86, 164, 100, 109, 198, 173, 186, 3, 64, 52, 217, 226, 250, 124, 123,
        5, 202, 38, 147, 118, 126, 255, 82, 85, 212, 207, 206, 59, 227, 47, 16, 58, 17, 182, 189, 28, 42,
        223, 183, 170, 213, 119, 248, 152, 2, 44, 154, 163, 70, 221, 153, 101, 155, 167, 43, 172, 9,
        129, 22, 39, 253, 19, 98, 108, 110, 79, 113, 224, 232, 178, 185, 112, 104, 218, 246, 97, 228,
        251, 34, 242, 193, 238, 210, 144, 12, 191, 179, 162, 241, 81, 51,145, 235, 249, 14, 239, 107,
        49, 192, 214, 31, 181, 199, 106, 157, 184, 84, 204, 176, 115, 121, 50, 45, 127, 4, 150, 254,
        138, 236, 205, 93, 222, 114, 67, 29, 24, 72, 243, 141, 128, 195, 78, 66, 215, 61, 156, 180
    };

    void InitHashTable()
    {
        int HashTableSize = 2 * sizeof(HashTblBase) / sizeof(HashTblBase[0]);

        HashTbl = new int[HashTableSize];
        for (int Index = 0; Index < HashTableSize; Index++)
        {
            HashTbl[Index] = HashTblBase[Index % (HashTableSize / 2)];
        }
    }

    void DeleteHashTable()
    {
        if (HashTbl)
        {
            delete HashTbl;
        }
    }

    static float Perlin(float X, float Y, float Z)
    {
        InitHashTable();

        // calculate a location of unit cube.
        int UnitCubeX = static_cast<int>(X) & 255;
        int UnitCubeY = static_cast<int>(Y) & 255;
        int UnitCubeZ = static_cast<int>(Z) & 255;

        // coordinate in identity cube.
        float OffsetX = X - static_cast<int>(X);
        float OffsetY = Y - static_cast<int>(Y);
        float OffsetZ = Z - static_cast<int>(Z);

        // create weight for lerp.
        float U = Fade(OffsetX);
        float V = Fade(OffsetY);
        float W = Fade(OffsetZ);

        // create hashes.
        int AAA = HashTbl[HashTbl[HashTbl[UnitCubeX    ] + UnitCubeY    ] + UnitCubeZ    ];
        int ABA = HashTbl[HashTbl[HashTbl[UnitCubeX    ] + UnitCubeY + 1] + UnitCubeZ    ];
        int AAB = HashTbl[HashTbl[HashTbl[UnitCubeX    ] + UnitCubeY    ] + UnitCubeZ + 1];
        int ABB = HashTbl[HashTbl[HashTbl[UnitCubeX    ] + UnitCubeY + 1] + UnitCubeZ + 1];
        int BAA = HashTbl[HashTbl[HashTbl[UnitCubeX + 1] + UnitCubeY    ] + UnitCubeZ    ];
        int BBA = HashTbl[HashTbl[HashTbl[UnitCubeX + 1] + UnitCubeY + 1] + UnitCubeZ    ];
        int BAB = HashTbl[HashTbl[HashTbl[UnitCubeX + 1] + UnitCubeY    ] + UnitCubeZ + 1];
        int BBB = HashTbl[HashTbl[HashTbl[UnitCubeX + 1] + UnitCubeY + 1] + UnitCubeZ + 1];

        // create perlin noise.
        float X1, X2, Y1, Y2;
        X1 = Lerp(Grad(AAA, OffsetX, OffsetY, OffsetZ),
                Grad(BAA, OffsetX - 1, OffsetY, OffsetZ),
                U);
        X2 = Lerp(Grad(ABA, OffsetX, OffsetY - 1, OffsetZ),
                Grad(BBA, OffsetX - 1, OffsetY - 1, OffsetZ),
                U);
        Y1 = Lerp(X1, X2, V);
        X1 = Lerp(Grad(AAB, OffsetX, OffsetY, OffsetZ - 1),
                Grad(BAB, OffsetX - 1, OffsetY, OffsetZ - 1),
                U);
        X2 = Lerp(Grad(ABB, OffsetX, OffsetY - 1, OffsetZ - 1),
                Grad(BBB, OffsetX - 1, OffsetY - 1, OffsetZ - 1),
                U);
        Y2 = Lerp(X1, X2, V);

        float Result = (Lerp(Y1, Y2, W) + 1) / 2;

        DeleteHashTable();

        return Result;
    }

    float Perlin(float X, float Y, float Z, int Octaves, float Persistence)
    {
        float Total = 0.0f;
        float Frequency = 1.0f;
        float Amplitude = 1.0f;
        float MaxValue = 1.0f;

        for (int O = 0; O < Octaves; O++) {
            Total += Perlin(X * Frequency, Y * Frequency, Z * Frequency) * Amplitude;
            MaxValue += Amplitude;
            Amplitude *= Persistence;
            Frequency *= 2.0f;
        }

        return Total / MaxValue;
    }

    float ScaleToHashTblSize(float Value)
    {
        return Value * sizeof(HashTblBase);
    }
}   // namespace

UPerlinNoiseBPLibrary::UPerlinNoiseBPLibrary(const FObjectInitializer& ObjectInitializer)
: Super(ObjectInitializer)
{
}

void UPerlinNoiseBPLibrary::OctavePerlinNoise1D(float& Out, float X,
                                                int Octaves, float Persistence)
{
    float ScaledX = ScaleToHashTblSize(X);

    Out = Perlin(ScaledX, 0.0f, 0.0f, Octaves, Persistence);
}

void UPerlinNoiseBPLibrary::OctavePerlinNoise2D(float& Out, float X, float Y,
                                                int Octaves, float Persistence)
{
    float ScaledX = ScaleToHashTblSize(X);
    float ScaledY = ScaleToHashTblSize(Y);

    Out = Perlin(ScaledX, ScaledY, 0.0f, Octaves, Persistence);
}

void UPerlinNoiseBPLibrary::OctavePerlinNoise3D(float& Out, float X, float Y, float Z,
                                                int Octaves, float Persistence)
{
    float ScaledX = ScaleToHashTblSize(X);
    float ScaledY = ScaleToHashTblSize(Y);
    float ScaledZ = ScaleToHashTblSize(Z);

    Out = Perlin(ScaledX, ScaledY, ScaledZ, Octaves, Persistence);
}

void UPerlinNoiseBPLibrary::PerlinNoise1D(float& Out, float X,
                                          float Frequency, float Amplitude)
{
    float ScaledX = ScaleToHashTblSize(X);

    Out = Perlin(ScaledX * Frequency, 0.0f, 0.0f) * Amplitude;
}

void UPerlinNoiseBPLibrary::PerlinNoise2D(float& Out, float X, float Y,
                                          float Frequency, float Amplitude)
{
    float ScaledX = ScaleToHashTblSize(X);
    float ScaledY = ScaleToHashTblSize(Y);

    Out = Perlin(ScaledX * Frequency, ScaledY * Frequency, 0.0f) * Amplitude;
}

void UPerlinNoiseBPLibrary::PerlinNoise3D(float& Out, float X, float Y, float Z,
                                          float Frequency, float Amplitude)
{
    float ScaledX = ScaleToHashTblSize(X);
    float ScaledY = ScaleToHashTblSize(Y);
    float ScaledZ = ScaleToHashTblSize(Z);

    Out = Perlin(ScaledX * Frequency, ScaledY * Frequency, ScaledZ * Frequency) * Amplitude;
}

void UPerlinNoiseBPLibrary::AccumulatedPerlinNoise1D(float& Out,
                                                     const TArray<float>& Frequencies,
                                                     const TArray<float>& Amplitudes,
                                                     float X)
{
    if (Frequencies.Num() != Amplitudes.Num())
    {
        UE_LOG(LogTemp, Error,
               TEXT("Number of elements in Frequences and Amplitudes must be same. (Frequences: %d, Amplitudes: %d)"),
               Frequencies.Num(), Amplitudes.Num());
        Out = 0.0f;
        return;
    }

    float ScaledX = ScaleToHashTblSize(X);

    Out = 0.0f;
    for (int Index = 0; Index < Frequencies.Num(); ++Index)
    {
        Out += Perlin(ScaledX * Frequencies[Index], 0.0f, 0.0f) * Amplitudes[Index];
    }
}

void UPerlinNoiseBPLibrary::AccumulatedPerlinNoise2D(float& Out,
                                                     const TArray<float>& Frequencies,
                                                     const TArray<float>& Amplitudes,
                                                     float X, float Y)
{
    if (Frequencies.Num() != Amplitudes.Num())
    {
        UE_LOG(LogTemp, Error,
               TEXT("Number of elements in Frequences and Amplitudes must be same. (Frequences: %d, Amplitudes: %d)"),
               Frequencies.Num(), Amplitudes.Num());
        Out = 0.0f;
        return;
    }

    float ScaledX = ScaleToHashTblSize(X);
    float ScaledY = ScaleToHashTblSize(Y);

    Out = 0.0f;
    for (int Index = 0; Index < Frequencies.Num(); ++Index)
    {
        Out += Perlin(ScaledX * Frequencies[Index], ScaledY * Frequencies[Index], 0.0f) * Amplitudes[Index];
    }
}

void UPerlinNoiseBPLibrary::AccumulatedPerlinNoise3D(float& Out,
                                                     const TArray<float>& Frequencies,
                                                     const TArray<float>& Amplitudes,
                                                     float X, float Y, float Z)
{
    if (Frequencies.Num() != Amplitudes.Num())
    {
        UE_LOG(LogTemp, Error,
               TEXT("Number of elements in Frequences and Amplitudes must be same. (Frequences: %d, Amplitudes: %d)"),
               Frequencies.Num(), Amplitudes.Num());
        Out = 0.0f;
        return;
    }

    float ScaledX = ScaleToHashTblSize(X);
    float ScaledY = ScaleToHashTblSize(Y);
    float ScaledZ = ScaleToHashTblSize(Z);

    Out = 0.0f;
    for (int Index = 0; Index < Frequencies.Num(); ++Index)
    {
        Out += Perlin(ScaledX * Frequencies[Index], ScaledY * Frequencies[Index], ScaledZ * Frequencies[Index]) * Amplitudes[Index];
    }
}
