enum // Hissatsu-Type
{
	HWT_TEST_01,
	HWT_TEST_02,
	HWT_TEST_03,

	HWT_G,
	HWT_I,
	HWT_J,
	HWT_P,
	HWT_E,
	HWT_A,
	HWT_U,
	HWT_C,
	HWT_F,
	HWT_S,
	HWT_R,

	HWT_GI,
	HWT_PJ,
	HWT_GJ,
	HWT_IJ,
	HWT_PI,
	HWT_GP,
	HWT_EJ,
	HWT_AE,
	HWT_SR,
	HWT_UP,
	HWT_EC,
	HWT_IR,
	HWT_UA,
	HWT_FS,
	HWT_FE,

	HWT_MAX,
};
extern char *LHissatsuTypes[HWT_MAX];

void PzHissatsu(int mapIndex, int hissatsuType);
