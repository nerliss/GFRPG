// Oleksandr Tkachov 2021-2026


#include "Widgets/Inventory/RPGInventoryTooltipWidget.h"

URPGInventoryTooltipWidget::URPGInventoryTooltipWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ItemName = FText::FromString("Item Name");
	ItemDescription = FText::FromString("Item Description");
	ItemThumbnailToSet = nullptr;
}