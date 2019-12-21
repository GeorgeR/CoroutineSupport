using UnrealBuildTool;

public class CoroutineSupport : ModuleRules
{
	public CoroutineSupport(ReadOnlyTargetRules Target)
        : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
			});

        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "Engine",
            });

        var bSupported = Target.CppStandard >= CppStandardVersion.Cpp17;
        PublicDefinitions.Add(string.Format("WITH_CO_AWAIT={0}", bSupported ? 1 : 0));
    }
}
