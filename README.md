# ue4-richtextblocktooltip-sample

![UE4](https://img.shields.io/badge/UE4-4.25+-blue)
[![GitHub license](https://img.shields.io/badge/license-MIT-blue.svg)](https://raw.githubusercontent.com/Nauja/ue4-richtextblocktooltip-sample/master/LICENSE)

Sample of writing a custom RichTextBlock decorator for displaying a tooltip.

![Preview](https://github.com/Nauja/ue4-richtextblocktooltip-sample/raw/media/preview.gif)

## Table of contents:

- [Writing a custom **URichTextBlockDecorator**](#writing-a-custom-urichtextblockdecorator)
- [Setting up HUD](#setting-up-hud)
- [Use this custom decorator in Blueprint](#use-this-custom-decorator-in-blueprint)

## Writing a custom **URichTextBlockDecorator**

Based on [Advanced Text Styling with Rich Text Block](https://www.unrealengine.com/en-US/tech-blog/advanced-text-styling-with-rich-text-block) documentation, you can create and add your own **URichTextBlockDecorator** class on a **URichTextBlock** to handle custom markup. There is nothing complicated here, and you can take example on the builtin **URichTextBlockImageDecorator** class.

For this sample, I created a new decorator named **USampleRichTextBlockTooltipDecorator** that will replace any `<tooltip text="Some infos">Some text</>` tag in our text by a **UTextBlock** that can be hovered to display a tooltip.

Here is the definition in **SampleRichTextBlockTooltipDecorator.h**:

```cpp
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
```

**CreateDecorator** is the only function that you need to implement on this class. This must return an instance of **FRichTextDecorator** that will take care of parsing and building the widget to render. I also added the two properties **TextStyle** and **TooltipTextStyle** editable in blueprint to allow for styling the text and the tooltip text independently.

Here is the implementation in **SampleRichTextBlockTooltipDecorator.cpp**:

```cpp
#include "SampleRichTextBlockTooltipDecorator.h"
#include "Widgets/SToolTip.h"
#include "Widgets/Text/STextBlock.h"

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
```

Again, this is largerly copied from the code of **URichTextBlockImageDecorator**. As you can see, the implementation of **CreateDecorator** is really simple as it only return a new instance of **FSampleRichInlineTooltip**.

**FSampleRichInlineTooltip** is the class that handle everything. The **Supports** function is called to know if the decorator can be used for a tag, returns **true** only if the tag's name is "tooltip" and if there is a "text" attribute. Then, **CreateDecoratorWidget** is called, and this is where we need to build our custom widget that will be displayed on screen.

## Setting up HUD

The **SampleMap** map has only the bare minimum. For the purpose of this sample, it only adds **WBP_SampleHUD** to viewport (see Level Blueprint):

![LevelBlueprint](https://github.com/Nauja/ue4-richtextblocktooltip-sample/raw/media/editor-levelblueprint.png)

**WBP_SampleHUD** contains a **URichTextBlock** widget that we will add our custom decorator class to:

![HUDWidget](https://github.com/Nauja/ue4-richtextblocktooltip-sample/raw/media/editor-hud-widget.png)

## Use this custom decorator in Blueprint

The next step is to subclass **USampleRichTextBlockTooltipDecorator** as a Blueprint so we can customize it and assign it in Blueprint.

## License

Licensed under the [MIT](LICENSE) License.
