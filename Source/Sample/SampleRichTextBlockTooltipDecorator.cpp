
#include "SampleRichTextBlockTooltipDecorator.h"
#include "Widgets/SToolTip.h"
#include "Widgets/Text/STextBlock.h"


// Class charged of creating the inline tooltip
class FSampleRichInlineTooltip : public FRichTextDecorator
{
public:
	FSampleRichInlineTooltip(URichTextBlock* InOwner)
		: FRichTextDecorator(InOwner)
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
	virtual TSharedPtr<SWidget> CreateDecoratorWidget(const FTextRunInfo& RunInfo, const FTextBlockStyle& TextStyle) const override
	{
		return SNew(STextBlock)
			.Text(RunInfo.Content)
			.TextStyle(&TextStyle)
			.ToolTip(SNew(SToolTip)
			[
				SNew(STextBlock)
				.Text(FText::FromString(RunInfo.MetaData[TEXT("text")]))
				.TextStyle(&TextStyle)
			]);
	}
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
	return MakeShareable(new FSampleRichInlineTooltip(InOwner));
}

/////////////////////////////////////////////////////
