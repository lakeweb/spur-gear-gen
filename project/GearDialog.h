#pragma once


class CGearsDialog : public CPaneDialog
{
public:
	enum { IDD = IDS_GEAR_DLG };
	CGearsDialog( );
	virtual ~CGearsDialog( );

	CComboBox cGeardlgPitchCombo;

	int valGearPitch;
	int valToothCount;
	double valPitchAngle;

	SP_gear_params_t sp_gear;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg int OnCreate( LPCREATESTRUCT lpCreateStruct );
	afx_msg void OnSize( UINT nType, int cx, int cy );
	afx_msg void OnCbnSelchangeGearPtichAng();
	afx_msg LRESULT HandleInitDialog( WPARAM wParam, LPARAM lParam );
public:
	afx_msg void OnEnChangeGearToothCnt();
	afx_msg void OnEnChangeGearPitch();
};
