#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"

#include "PerlinNoiseBPLibrary.generated.h"

UCLASS()
class UPerlinNoiseBPLibrary : public UBlueprintFunctionLibrary
{
    GENERATED_UCLASS_BODY()

    UFUNCTION(BluePrintCallable, Category="Noise|PerlinNoise")
    static void OctavePerlinNoise1D(float& Out, float X = 0.0f,
                                    int Octaves = 1, float Persistence = 1.0f);

    UFUNCTION(BluePrintCallable, Category="Noise|PerlinNoise")
    static void OctavePerlinNoise2D(float& Out, float X = 0.0f, float Y = 0.0f,
                                    int Octaves = 1, float Persistence = 1.0f);

    UFUNCTION(BluePrintCallable, Category="Noise|PerlinNoise")
    static void OctavePerlinNoise3D(float& Out, float X = 0.0f, float Y = 0.0f, float Z = 0.0f,
                                    int Octaves = 1, float Persistence = 1.0f);

    UFUNCTION(BluePrintCallable, Category="Noise|PerlinNoise")
    static void PerlinNoise1D(float& Out, float X = 0.0f,
                              float Frequency = 1.0f, float Amplitude = 1.0f);

    UFUNCTION(BluePrintCallable, Category="Noise|PerlinNoise")
    static void PerlinNoise2D(float& Out, float X = 0.0f, float Y = 0.0f,
                              float Frequency = 1.0f, float Amplitude = 1.0f);

    UFUNCTION(BluePrintCallable, Category="Noise|PerlinNoise")
    static void PerlinNoise3D(float& Out, float X = 0.0f, float Y = 0.0f, float Z = 0.0f,
                              float Frequency = 1, float Amplitude = 1.0f);

    UFUNCTION(BluePrintCallable, Category="Noise|PerlinNoise")
    static void AccumulatedPerlinNoise1D(float& Out,
                                         const TArray<float>& Frequencies,
                                         const TArray<float>& Amplitudes,
                                         float X = 0.0f);
    
    UFUNCTION(BluePrintCallable, Category="Noise|PerlinNoise")
    static void AccumulatedPerlinNoise2D(float& Out,
                                         const TArray<float>& Frequencies,
                                         const TArray<float>& Amplitudes,
                                         float X = 0.0f, float Y = 0.0f);

    UFUNCTION(BluePrintCallable, Category="Noise|PerlinNoise")
    static void AccumulatedPerlinNoise3D(float& Out,
                                         const TArray<float>& Frequencies,
                                         const TArray<float>& Amplitudes,
                                         float X = 0.0f, float Y = 0.0f, float Z = 0.0f);
};