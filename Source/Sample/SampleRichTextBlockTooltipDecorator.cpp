
#include "SampleRichTextBlockTooltipDecorator.h"
#include "Widgets/SToolTip.h"
#include "Widgets/Text/STextBlock.h"

/*
* RichTExt example
* Try to hover <tooltip text="I am a tooltip">this word</> or <tooltip text="I am also a tooltip">this one</>
*/

// Class charged of creating the inline tooltip
class FSampleRichInlineTooltip : public FRichTextDecorator
{
public:
	FSampleRichInlineTooltip(URichTextBlock* InOwner, const FTextBlockStyle& InTextStyle, const FTextBlockStyle& InTooltipTextStyle)
		: FRichTextDecorator(InOwner)
		, TextStyle(InTextStyle)
		, TooltipTextStyle(InTooltipTextStyle)
	{
	}

	// Only valid if text is: <tooltip text="Some infos">Some text</>
	virtual bool Supports(const FTextRunParseResults& RunParseResult, const FString& Text) const override
	{
		return RunParseResult.Name == TEXT("tooltip") && RunParseResult.MetaData.Contains(TEXT("text"));
	}

protected:
	/**
	 * Create a STextBlock with a tooltip text.
	 * 
	 * For <tooltip text="Some infos">Some text</>:
	 * - RunInfo.Content is "Some text"
	 * - RunInfo.MetaData[TEXT("text")] is "Some infos"
	 */
	virtual TSharedPtr<SWidget> CreateDecoratorWidget(const FTextRunInfo& InRunInfo, const FTextBlockStyle& InTextStyle) const override
	{
		return SNew(STextBlock)
			.Text(InRunInfo.Content)
			.TextStyle(&TextStyle)
			.ToolTip(SNew(SToolTip)
			[
				SNew(STextBlock)
				.Text(FText::FromString(InRunInfo.MetaData[TEXT("text")]))
				.TextStyle(&TooltipTextStyle)
			]);
	}

private:
	FTextBlockStyle TextStyle;
	FTextBlockStyle TooltipTextStyle;
};

/////////////////////////////////////////////////////
// USampleRichTextBlockTooltipDecorator

USampleRichTextBlockTooltipDecorator::USampleRichTextBlockTooltipDecorator(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

// Return our custom class for creating the inline widget
TSharedPtr<ITextDecorator> USampleRichTextBlockTooltipDecorator::CreateDecorator(URichTextBlock* InOwner)
{
	return MakeShareable(new FSampleRichInlineTooltip(InOwner, TextStyle, TooltipTextStyle));
}

/////////////////////////////////////////////////////
