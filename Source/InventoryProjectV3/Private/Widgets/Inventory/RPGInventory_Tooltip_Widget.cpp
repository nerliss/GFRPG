// Oleksandr Tkachov 2022-2023


#include "Widgets/Inventory/RPGInventory_Tooltip_Widget.h"

URPGInventory_Tooltip_Widget::URPGInventory_Tooltip_Widget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ItemName = FText::FromString("Item Name");
	ItemDescription = FText::FromString("Item Description");
	ItemThumbnailToSet = nullptr;
}