
�y�{�v���W�F�N�g�̂�������\���z

UE4��Unity�̍\�����Q�l�ɂ��Ă��āA
�P�̃I�u�W�F�N�g�i�A�N�^�[�j�ɐF�X�ȋ@�\�i�R���|�[�l���g�j���������Ď�������݌v�ɂȂ��Ă��܂��B

�Ⴆ�΃��f����\���������ꍇ�A
���f���p�̃A�N�^�[�𐶐����āA���̃A�N�^�[�Ƀ��f���\���p�̃R���|�[�l���g���������邱�ƂŎ������Ă��܂��B

Actor* actor = new Actor();
new MeshComponent(actor, LoadMesh("data/model/sample.obj"));

���̂Q�s�����������ɏ��������ŃT���v�����f�����\���ł���͂��ł��B


����ɁA�ړ����鏈����ǉ��������Ȃ����ꍇ�A�ړ��p�̃R���|�[�l���g�������

Actor* actor = new Actor();
new MeshComponent(actor, LoadMesh("data/model/sample.obj"));
new MoveComponent(actor);

�Ƃ���΃��f����\�����Ȃ���ړ�����I�u�W�F�N�g�������ł��܂��B


�C���[�W�}�͂���Ȋ����ł��B

             [Actor]
               ��
       ������������������
[MeshComponent]   [MoveComponent]


�������邱�ƂŁA�_�C�������h�p����������_��Ȑ݌v���ł���Ǝv���Ă܂��B
�Ƃ͂���������{�v���W�F�N�g�ɉ�������i������Ă����͓̂���Ǝv���̂ŁA
���̕ӂ͎��R�ɉ������Ďg���₷���悤�ɂ��ĖႦ��΂Ǝv���܂��B


�y���ӎ����z
�E��{�I��FBX�t�@�C���̓��f���̃X�P�[�����傫���̂ŏo�͂̍ۂ�0.01�ɐݒ肵�Ȃ����܂��B
�E�{�[���̐��͍ő�12�܂Łi�V�F�[�_�[�̒萔�o�b�t�@�ő�T�C�Y�𒴂����Ⴄ�̂Łj
�E12�ȏ�ݒ肵�����ꍇ�́ACPU���Ōv�Z����悤�ɍ\����ς��邩�A���f���𕪊����đΉ�����B
�E���}��������̂ŃA�j���[�V���������̃��f�����ƃo�O��BSkinnedMesh.cpp��M�邩�A�A�j���[�V���������͂���܂Œʂ�obj�ŕ\�����đΉ�����B
�E����̗��R�ŁA�P�t�@�C���ɂ��P���f���P�e�N�X�`���ɂ����Ή����ĂȂ��B

�y�e�t�@�C���̐����z
Bone.cpp / Bone.h
���X�L�����b�V���ɕK�v�ȃ{�[���i���O�j�̏������s���B

FBXLoader.cpp / FBXLoader.h
��FBX�t�@�C���i��A�X�L�[�^���j�̓ǂݍ��݂��s���B�{�v���W�F�N�g�ň�ԕ��G�ȂƂ��B
�@FBX�t�@�C���̍\���͌y���������Ƃ����ق��������Ǝv���B�Q�l�T�C�g��
�@https://code.blender.org/2013/08/fbx-binary-file-format-specification/
�@���Ȃ݂ɁA�v���O�����̓l�b�g�ɏオ���Ă��T���v�����Q�l�ɂ��Ă�B

miniz.cpp / miniz.h
��FBX�t�@�C���ǂݍ��݂̍ۂɈ��k�o�C�i���̉𓀂��s�����C�u�����B
�@���삶��Ȃ��B

SkinnedMesh.cpp / SkinnedMesh.h
���ǂ݂���FBX�t�@�C�����N���X���������́B

SkinnedMeshManager.cpp / SkinnedMeshManager.h
��SkinnedMesh�N���X�̊Ǘ����s���B

SkinnedMeshComponent.cpp / SkinnedMeshComponent.h
���X�L�����b�V�����s�����߂̃R���|�[�l���g�B
�@��{�I�ɂ͂������A�N�^�[�ɂ��ăX�L�����b�V�����Q�[�����Ŏ�������B


�y�g�����z
GameSystem.cpp��39�s�ڂ��Q�l�B���������Ɉȉ��̏���������B
�X�V�A�`��A�I�������Ȃǂ͏���ɂ��Ă����̂œ��ɕK�v�Ȃ��B

	// �@�A�N�^�[�𐶐�
	Actor* actor = new Actor();

	// �A�X�L�����b�V���R���|�[�l���g�𐶐�
	SkinnedMeshComponent* skinMesh = new SkinnedMeshComponent(actor);

	// �B���b�V���i���f���j�f�[�^�����[�h���R���|�[�l���g�ɃZ�b�g
	SkinnedMesh* mesh = LoadSkinnedMesh("data/model/model_player_01.fbx");
	skinMesh->SetMesh(mesh);

	// �C�Đ�����A�j���[�V�����X�^�b�N�̔ԍ����Z�b�g
	skinMesh->SetAnimStack(1);

	// �D�A�j���[�V�������Đ��i�Ă΂Ȃ��Ă������j
	skinMesh->PlayAnimation();