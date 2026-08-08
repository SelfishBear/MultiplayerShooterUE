// Fill out your copyright notice in the Description page of Project Settings.


#include "Utils/MS_GraphicsFunctionLibrary.h"

#include "HAL/IConsoleManager.h"
#include "Scalability.h"

namespace
{
	struct FSavedConsoleVariable
	{
		IConsoleVariable* Variable = nullptr;
		FString Value;
		EConsoleVariableFlags SetBy = ECVF_SetByConstructor;
	};

	TOptional<Scalability::FQualityLevels> SavedQualityLevels;
	TArray<FSavedConsoleVariable> SavedConsoleVariables;

	const TPair<const TCHAR*, const TCHAR*> PerformanceSettings[] =
	{
		{TEXT("r.ScreenPercentage"), TEXT("10")},
		{TEXT("r.PostProcessAAQuality"), TEXT("0")},
		{TEXT("r.MotionBlurQuality"), TEXT("0")},
		{TEXT("r.BloomQuality"), TEXT("0")},
		{TEXT("r.DepthOfFieldQuality"), TEXT("0")},
		{TEXT("r.LensFlareQuality"), TEXT("0")},
		{TEXT("r.EyeAdaptationQuality"), TEXT("0")},
		{TEXT("r.AmbientOcclusionLevels"), TEXT("0")},
		{TEXT("r.SSR.Quality"), TEXT("0")},
		{TEXT("r.VolumetricFog"), TEXT("0")},
		{TEXT("r.Fog"), TEXT("0")},
		{TEXT("r.ParticleLightQuality"), TEXT("0")},
		{TEXT("r.RefractionQuality"), TEXT("0")},
		{TEXT("r.MaxAnisotropy"), TEXT("0")},
		{TEXT("r.MipMapLODBias"), TEXT("8")},
		{TEXT("r.ViewDistanceScale"), TEXT("0.1")},
		{TEXT("r.SkeletalMeshLODBias"), TEXT("10")},
		{TEXT("r.StaticMeshLODDistanceScale"), TEXT("10")},
		{TEXT("foliage.DensityScale"), TEXT("0")},
		{TEXT("grass.DensityScale"), TEXT("0")}
	};

	void SaveAndApplyPerformanceConsoleVariables()
	{
		IConsoleManager& ConsoleManager = IConsoleManager::Get();

		for (const TPair<const TCHAR*, const TCHAR*>& Setting : PerformanceSettings)
		{
			IConsoleVariable* Variable = ConsoleManager.FindConsoleVariable(Setting.Key);
			if (Variable == nullptr)
			{
				continue;
			}

			SavedConsoleVariables.Add({
				Variable,
				Variable->GetString(),
				static_cast<EConsoleVariableFlags>(Variable->GetFlags() & ECVF_SetByMask)
			});
			Variable->Set(Setting.Value, ECVF_SetByConsole);
		}
	}

	void RestoreConsoleVariables()
	{
		for (const FSavedConsoleVariable& SavedVariable : SavedConsoleVariables)
		{
			// Set() ignores a value coming from a lower-priority source. Replace the
			// temporary SetByConsole priority first, then restore the original value.
			const EConsoleVariableFlags FlagsWithoutSetBy = static_cast<EConsoleVariableFlags>(
				SavedVariable.Variable->GetFlags() & ~ECVF_SetByMask);
			SavedVariable.Variable->SetFlags(static_cast<EConsoleVariableFlags>(
				FlagsWithoutSetBy | SavedVariable.SetBy));
			SavedVariable.Variable->Set(*SavedVariable.Value, SavedVariable.SetBy);
		}

		SavedConsoleVariables.Reset();
	}
}

void UMS_GraphicsFunctionLibrary::MakeGraphicsBad()
{
	if (!SavedQualityLevels.IsSet())
	{
		SavedQualityLevels = Scalability::GetQualityLevels();
		SaveAndApplyPerformanceConsoleVariables();
	}

	Scalability::FQualityLevels LowestQuality;
	LowestQuality.SetFromSingleQualityLevel(0);

	LowestQuality.ResolutionQuality = 10.0f;
	Scalability::SetQualityLevels(LowestQuality);

	for (const TPair<const TCHAR*, const TCHAR*>& Setting : PerformanceSettings)
	{
		if (IConsoleVariable* Variable = IConsoleManager::Get().FindConsoleVariable(Setting.Key))
		{
			Variable->Set(Setting.Value, ECVF_SetByConsole);
		}
	}
}

void UMS_GraphicsFunctionLibrary::MakeGraphicsGood()
{
	if (!SavedQualityLevels.IsSet())
	{
		return;
	}

	Scalability::SetQualityLevels(SavedQualityLevels.GetValue());
	RestoreConsoleVariables();
	SavedQualityLevels.Reset();
}
