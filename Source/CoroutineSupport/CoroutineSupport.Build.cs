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
	}
}
