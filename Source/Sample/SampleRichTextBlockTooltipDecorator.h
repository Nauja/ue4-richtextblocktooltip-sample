#pragma once

#include "CoreMinimal.h"
#include "Framework/Text/ITextDecorator.h"
#include "Components/RichTextBlockDecorator.h"

#include "SampleRichTextBlockTooltipDecorator.generated.h"

/**
 * Allows you to setup a tooltip decorator.
 * This can be subclassed as a blueprint.
 *
 * Understands the format <tooltip text="Some infos">Some text</>
 */
UCLASS(Abstract, Blueprintable)
class USampleRichTextBlockTooltipDecorator : public URichTextBlockDecorator
{
	GENERATED_BODY()

public:
	USampleRichTextBlockTooltipDecorator(const FObjectInitializer& ObjectInitializer);

	virtual TSharedPtr<ITextDecorator> CreateDecorator(URichTextBlock* InOwner) override;
	
protected:
	// Style used for text
	UPROPERTY(EditAnywhere, Category=Appearance)
	FTextBlockStyle TextStyle;

	// Style used for tooltip text
	UPROPERTY(EditAnywhere, Category=Appearance)
	FTextBlockStyle TooltipTextStyle;
};
