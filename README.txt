
【本プロジェクトのざっくり構成】

UE4やUnityの構成を参考にしていて、
１つのオブジェクト（アクター）に色々な機能（コンポーネント）をくっつけて実装する設計になっています。

例えばモデルを表示したい場合、
モデル用のアクターを生成して、そのアクターにモデル表示用のコンポーネントをくっつけることで実装しています。

Actor* actor = new Actor();
new MeshComponent(actor, LoadMesh("data/model/sample.obj"));

この２行を初期化時に書くだけでサンプルモデルが表示できるはずです。


さらに、移動する処理を追加したくなった場合、移動用のコンポーネントを作って

Actor* actor = new Actor();
new MeshComponent(actor, LoadMesh("data/model/sample.obj"));
new MoveComponent(actor);

とすればモデルを表示しながら移動するオブジェクトを実装できます。


イメージ図はこんな感じです。

             [Actor]
               ┃
       ┏━━━┻━━━┓
[MeshComponent]   [MoveComponent]


こうすることで、ダイヤモンド継承を回避しつつ柔軟な設計ができると思ってます。
とはいえ今から本プロジェクトに沿った作品を作っていくのは難しいと思うので、
この辺は自由に改造して使いやすいようにして貰えればと思います。


【注意事項】
・基本的にFBXファイルはモデルのスケールが大きいので出力の際に0.01に設定しなおします。
・ボーンの数は最大12個まで（シェーダーの定数バッファ最大サイズを超えちゃうので）
・12個以上設定したい場合は、CPU側で計算するように構成を変えるか、モデルを分割して対応する。
・取り急ぎ作ったのでアニメーション無しのモデルだとバグる。SkinnedMesh.cppを弄るか、アニメーション無しはこれまで通りobjで表示して対応する。
・同上の理由で、１ファイルにつき１モデル１テクスチャにしか対応してない。

【各ファイルの説明】
Bone.cpp / Bone.h
→スキンメッシュに必要なボーン（リグ）の処理を行う。

FBXLoader.cpp / FBXLoader.h
→FBXファイル（非アスキー型式）の読み込みを行う。本プロジェクトで一番複雑なとこ。
　FBXファイルの構造は軽く理解しといたほうがいいと思う。参考サイト↓
　https://code.blender.org/2013/08/fbx-binary-file-format-specification/
　ちなみに、プログラムはネットに上がってたサンプルを参考にしてる。

miniz.cpp / miniz.h
→FBXファイル読み込みの際に圧縮バイナリの解凍を行うライブラリ。
　自作じゃない。

SkinnedMesh.cpp / SkinnedMesh.h
→読みこんだFBXファイルをクラス化したもの。

SkinnedMeshManager.cpp / SkinnedMeshManager.h
→SkinnedMeshクラスの管理を行う。

SkinnedMeshComponent.cpp / SkinnedMeshComponent.h
→スキンメッシュを行うためのコンポーネント。
　基本的にはこいつをアクターにつけてスキンメッシュをゲーム内で実装する。


【使い方】
GameSystem.cppの39行目を参考。初期化時に以下の処理をする。
更新、描画、終了処理などは勝手にしてくれるので特に必要ない。

	// ①アクターを生成
	Actor* actor = new Actor();

	// ②スキンメッシュコンポーネントを生成
	SkinnedMeshComponent* skinMesh = new SkinnedMeshComponent(actor);

	// ③メッシュ（モデル）データをロード＆コンポーネントにセット
	SkinnedMesh* mesh = LoadSkinnedMesh("data/model/model_player_01.fbx");
	skinMesh->SetMesh(mesh);

	// ④再生するアニメーションスタックの番号をセット
	skinMesh->SetAnimStack(1);

	// ⑤アニメーションを再生（呼ばなくてもいい）
	skinMesh->PlayAnimation();