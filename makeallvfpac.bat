
echo "starting building ......"
set jobn=4
set jn=2

echo "starting building DS_MP_vf ......"
make\make_cmd\make -r -R p=UIS8910FF_DS_MP_vf job=%jobn% -j%jn% PID=VF new >err_qa
make\make_cmd\make -r -R p=UIS8910FF_DS_MP_vf job=%jobn% -j%jn% PID=VF new >err_qa2

make\make_cmd\make -r -R p=UIS8910FF_DS_MP_vf job=%jobn% -j%jn% PID=VF@23QUE55VF01 br=n m=nv_parameter > err_N_nv
make\make_cmd\make -r -R p=UIS8910FF_DS_MP_vf job=%jobn% -j%jn% PID=VF@23QUE55VF01 br=n image > err_N_image

echo "starting building SS_MP_vf ......"
make\make_cmd\make -r -R p=UIS8910FF_SS_MP_vf job=%jobn% -j%jn% PID=VF new >err_qa
make\make_cmd\make -r -R p=UIS8910FF_SS_MP_vf job=%jobn% -j%jn% PID=VF new >err_qa2

make\make_cmd\make -r -R p=UIS8910FF_SS_MP_vf job=%jobn% -j%jn% PID=VF@23QUE71VF01 br=n m=nv_parameter > err_N_nv
make\make_cmd\make -r -R p=UIS8910FF_SS_MP_vf job=%jobn% -j%jn% PID=VF@23QUE71VF01 br=n image > err_N_image

echo "starting building SS_MP_ORTH ......"
make\make_cmd\make -r -R p=UIS8910FF_SS_MP_ORTH job=%jobn% -j%jn% PID=ER@23QUE71OD00 new >err_qa
make\make_cmd\make -r -R p=UIS8910FF_SS_MP_ORTH job=%jobn% -j%jn% PID=ER@23QUE71OD00 new >err_qa2


echo "starting building dualsim_vf ......"
make\make_cmd\make -r -R p=UIS8910FF_mpphone_evt_dualsim_vf job=%jobn% -j%jn% PID=VF new >err_qa
make\make_cmd\make -r -R p=UIS8910FF_mpphone_evt_dualsim_vf job=%jobn% -j%jn% PID=VF new >err_qa2

make\make_cmd\make -r -R p=UIS8910FF_mpphone_evt_dualsim_vf job=%jobn% -j%jn% PID=VF@23QUE55VF01 br=n m=nv_parameter > err_N_nv
make\make_cmd\make -r -R p=UIS8910FF_mpphone_evt_dualsim_vf job=%jobn% -j%jn% PID=VF@23QUE55VF01 br=n image > err_N_image

echo "starting building singlesim_vf ......"
make\make_cmd\make -r -R p=UIS8910FF_mpphone_evt_singlesim_vf job=%jobn% -j%jn% PID=VF new >err_qa
make\make_cmd\make -r -R p=UIS8910FF_mpphone_evt_singlesim_vf job=%jobn% -j%jn% PID=VF new >err_qa2

make\make_cmd\make -r -R p=UIS8910FF_mpphone_evt_singlesim_vf job=%jobn% -j%jn% PID=VF@23QUE71VF01 br=n m=nv_parameter > err_N_nv
make\make_cmd\make -r -R p=UIS8910FF_mpphone_evt_singlesim_vf job=%jobn% -j%jn% PID=VF@23QUE71VF01 br=n image > err_N_image

echo "starting building mpphone_evt_singlesim_orth ......"
make\make_cmd\make -r -R p=UIS8910FF_mpphone_evt_singlesim_orth job=%jobn% -j%jn% PID=ER@23QUE71OD00 new >err_qa
make\make_cmd\make -r -R p=UIS8910FF_mpphone_evt_singlesim_orth job=%jobn% -j%jn% PID=ER@23QUE71OD00 new >err_qa2

echo "build finish ......"
