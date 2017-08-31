#include "neopolygonio.h"

NeoPolygonIO::NeoPolygonIO()
{

}

/*
 * CSVでfieldをファイルに保存したり読み出したりできるようにする
 * 1行あたり1つのNeoExPolygonを表して
 * フレームの内側、すでに置かれたピース等の見分けがつかないのでstateという数字を先頭に設置する
 *
 * 0: 初期でのフレーム
 * 1: 初期でヒントによって配置されたピース
 * 2: 未配置のピース
 * 3: 現時点で結合等を重ねたフレーム
 * 4: 配置済みのピース
 *
 * これに加えてstd::array<bool,50> is_placedが欲しいので
 * 末行にstate:5で{1 or 0}を50個書いて表現することとする
 */

void NeoPolygonIO::exportPolygon(procon::NeoField field, std::string file_path)
{

}

procon::NeoField NeoPolygonIO::importField(std::string file_path)
{
    procon::NeoField import_field;


}
