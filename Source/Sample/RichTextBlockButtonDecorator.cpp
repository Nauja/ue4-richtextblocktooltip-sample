
#include "RichTextBlockButtonDecorator.h"
#include "Widgets/SToolTip.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Input/SButton.h"

/////////////////////////////////////////////////////

class FRichInlineButton : public FRichTextDecorator
{
public:
	FRichInlineButton(URichTextBlock* InOwner, URichTextBlockButtonDecorator* InDecorator)
		: FRichTextDecorator(InOwner)
		, Decorator(InDecorator)
	{
	}

	// Only valid if text is: <tooltip text="Some infos">Some text</>
	virtual bool Supports(const FTextRunParseResults& RunParseResult, const FString& Text) const override
	{
		if (RunParseResult.Name == TEXT("button") && RunParseResult.MetaData.Contains(TEXT("id")))
		{
			const FTextRange& IdRange = RunParseResult.MetaData[TEXT("id")];
			const FString TagId = Text.Mid(IdRange.BeginIndex, IdRange.EndIndex - IdRange.BeginIndex);

			const bool bWarnIfMissing = false;
			return Decorator->FindButtonSet(*TagId, bWarnIfMissing) != nullptr;
		}

		return false;
	}

protected:

	virtual TSharedPtr<SWidget> CreateDecoratorWidget(const FTextRunInfo& InRunInfo, const FTextBlockStyle& InTextStyle) const override
	{
		const bool bWarnIfMissing = true;
		const FRichButtonRow* ButtonSet = Decorator->FindButtonSet(*InRunInfo.MetaData[TEXT("id")], bWarnIfMissing);

		if (!ensure(ButtonSet))
			return TSharedPtr<SWidget>();

		FString TextString;
		if (const FString* Text = InRunInfo.MetaData.Find(TEXT("text")))
			TextString = *Text;

		return SNew(SButton)
			.ButtonStyle(&ButtonSet->ButtonStyle)
			//.ForegroundColor(FLinearColor::White)
			//.OnClicked(this, &FPaperTileMapDetailsCustomization::OnPromoteToAssetButtonClicked)
			.Content()
			[
				SNew(STextBlock)
					.TextStyle(&ButtonSet->ButtonTextStyle)
					.Text(FText::FromString(TextString))
			];
	}

private:
	URichTextBlockButtonDecorator* Decorator;
};

/////////////////////////////////////////////////////
// URichTextBlockButtonDecorator

URichTextBlockButtonDecorator::URichTextBlockButtonDecorator(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

// Return our custom class for creating the inline widget
TSharedPtr<ITextDecorator> URichTextBlockButtonDecorator::CreateDecorator(URichTextBlock* InOwner)
{
	return MakeShareable(new FRichInlineButton(InOwner, this));
}

// find button set
FRichButtonRow* URichTextBlockButtonDecorator::FindButtonSet(FName TagOrId, bool bWarnIfMissing)
{
	if (ButtonSet == nullptr)
		return nullptr;

	FString ContextString;
	return ButtonSet->FindRow<FRichButtonRow>(TagOrId, ContextString, bWarnIfMissing);
}

/////////////////////////////////////////////////////
