#pragma once

#include "CoreMinimal.h"

#include "Engine/DataTable.h"

#include "Styling/SlateTypes.h"
#include "Fonts/SlateFontInfo.h"

#include "Framework/Text/ITextDecorator.h"
#include "Components/RichTextBlockDecorator.h"

#include "RichTextBlockButtonDecorator.generated.h"

/** Simple struct for rich text styles */
USTRUCT(Blueprintable, BlueprintType)
struct FRichButtonRow : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:

	// button style
	UPROPERTY(EditAnywhere, Category = Appearance)
	FButtonStyle ButtonStyle;

	//UPROPERTY(EditAnywhere, Category = Appearance)
	//FSlateColor ButtonForegroundColor;
	 
	// Style used for button's text 
	UPROPERTY(EditAnywhere, Category = Appearance)
	FTextBlockStyle ButtonTextStyle;
};

/**
 * Allows you to setup a button decorator.
 * This can be subclassed as a blueprint.
 *
 * Understands the format <button id="DataTableRowName" text="button text" />
 */
UCLASS(Abstract, Blueprintable)
class URichTextBlockButtonDecorator : public URichTextBlockDecorator
{
	GENERATED_BODY()

public:
	URichTextBlockButtonDecorator(const FObjectInitializer& ObjectInitializer);

	// create
	virtual TSharedPtr<ITextDecorator> CreateDecorator(URichTextBlock* InOwner) override;
	// find button set
	FRichButtonRow* FindButtonSet(FName TagOrId, bool bWarnIfMissing);

protected:


	UPROPERTY(EditAnywhere, Category = Appearance, meta = (RequiredAssetDataTags = "RowStructure=/Script/Sample.RichButtonRow"))
	TObjectPtr<class UDataTable> ButtonSet;
};
