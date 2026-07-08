#include "EditorUtilityFunctionLibrary.h"

bool UEditorUtilityFunctionLibrary::IsEditorBuild()
{
#if WITH_EDITOR
	return true;
#else
	return false;
#endif
}