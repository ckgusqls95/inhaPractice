#pragma once

cButton* Setup_BarGaugePopupBtn(cPopup* popup, D3DXVECTOR3 position);
void BarSliderMoveEvent(EventType message, cPopup* btn);
void BarSliderMoveEvent2(EventType message, cPopup* btn);

cButton* Setup_BarGaugePopupBtn_Legacy(cPopup* popup, D3DXVECTOR3 position);
void GaugeBarMoveEvent_Legacy(EventType message, cPopup* btn);