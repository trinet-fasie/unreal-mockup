// Fill out your copyright notice in the Description page of Project Settings.

using System.IO;
using UnrealBuildTool;

public class VR : ModuleRules
{
	private string ThirdPartyPath
    {
        get { return Path.GetFullPath(Path.Combine(ModuleDirectory, "../Common_3/ThirdParty/OpenSource/lua-5.3.5/")); }
    }

	public VR(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });
		//PrivateDependencyModuleNames.AddRange(new string[] {  });

		// Uncomment if you are using Slate UI
		//PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		//PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
		
		LoadLua(Target); // This functions loads Lua
	}

	private bool LoadLua(ReadOnlyTargetRules TargetRules)
    {
        bool isLibSupported = false;
        if ((Target.Platform == UnrealTargetPlatform.Win64))
        {
            isLibSupported = true;

            string LibrariesPath = Path.Combine("C:/Program Files/Lua/5.3/lib");

            PublicAdditionalLibraries.Add(Path.Combine(LibrariesPath, "lua5.3.lib"));

            PublicIncludePaths.Add("C:/Program Files/Lua/5.3/include");
        }

        // Check if we are on Windows
        //if ((Target.Platform == UnrealTargetPlatform.Win64) || (Target.Platform == UnrealTargetPlatform.Win32))
        //{
        //    isLibSupported = true;

        //    string PlatformString = (Target.Platform == UnrealTargetPlatform.Win64) ? "x64" : "x86"; // This string is either "x64" or "x86" so we can append it on the lib filename
        //    string LibrariesPath = Path.Combine(ThirdPartyPath, "Lua", "libraries");

        //    PublicAdditionalLibraries.Add(Path.Combine(LibrariesPath, "lua53." + PlatformString + ".lib"));

        //    PublicIncludePaths.Add(Path.Combine(ThirdPartyPath, "Lua", "includes"));
        //}

        PublicDefinitions.Add(string.Format("WITH_LUA_BINDING={0}", isLibSupported ? 1 : 0));

        return isLibSupported;
    }
}
