#include "testcheckcanprume.h"
#include "neofield.h"
#include "neopolygonviewer.h"
#include "Algorithm/beamsearch.h"
#include "probmaker.h"

TestCheckCanPrume::TestCheckCanPrume()
{

}

procon::NeoField TestCheckCanPrume::makeField(){
    procon::NeoField field;

    //elementaryframe
    polygon_i efPolygon_i{{{0 , 0}, {11 , 0}, {11 , 2}, {16 , 2}, {16 , 0} , {25 , 0}, {25 , 18}, {0 , 18}, {0 , 0}}};
    procon::NeoExpandedPolygon efNEP;
    efNEP.resetPolygonForce(efPolygon_i);
    field.setElementaryFrame({efNEP});

    //elementarypieces
    std::vector<procon::NeoExpandedPolygon> elementarypieces;
    polygon_i epPolygon_i;
    procon::NeoExpandedPolygon epNEP;
    //isplaced false
    bg::exterior_ring(epPolygon_i) = boost::assign::list_of<point_i>(2,1)(2,2)(5,2)(2,4)(0,3)(2,1);
    epNEP.resetPolygonForce(epPolygon_i);
    elementarypieces.push_back(epNEP);
    bg::exterior_ring(epPolygon_i) = boost::assign::list_of<point_i>(7,1)(11,1)(9,5)(6,4)(7,1);
    epNEP.resetPolygonForce(epPolygon_i);
    elementarypieces.push_back(epNEP);
    bg::exterior_ring(epPolygon_i) = boost::assign::list_of<point_i>(13,1)(16,1)(17,4)(13,4)(13,1);
    epNEP.resetPolygonForce(epPolygon_i);
    elementarypieces.push_back(epNEP);
    bg::exterior_ring(epPolygon_i) = boost::assign::list_of<point_i>(19,1)(23,1)(23,3)(20,3)(19,1);
    epNEP.resetPolygonForce(epPolygon_i);
    elementarypieces.push_back(epNEP);
    bg::exterior_ring(epPolygon_i) = boost::assign::list_of<point_i>(2,6)(5,5)(5,7)(3,8)(1,10)(2,6);
    epNEP.resetPolygonForce(epPolygon_i);
    elementarypieces.push_back(epNEP);
    bg::exterior_ring(epPolygon_i) = boost::assign::list_of<point_i>(6,6)(10,6)(10,9)(8,9)(8,7)(6,6);
    epNEP.resetPolygonForce(epPolygon_i);
    elementarypieces.push_back(epNEP);
    bg::exterior_ring(epPolygon_i) = boost::assign::list_of<point_i>(13,5)(18,6)(18,8)(13,8)(13,5);
    epNEP.resetPolygonForce(epPolygon_i);
    elementarypieces.push_back(epNEP);
    bg::exterior_ring(epPolygon_i) = boost::assign::list_of<point_i>(13,9)(17,9)(17,11)(13,11)(13,9);
    epNEP.resetPolygonForce(epPolygon_i);
    elementarypieces.push_back(epNEP);
    bg::exterior_ring(epPolygon_i) = boost::assign::list_of<point_i>(21,6)(24,5)(24,8)(25,8)(25,10)(20,10)(20,8)(21,6);
    epNEP.resetPolygonForce(epPolygon_i);
    elementarypieces.push_back(epNEP);
    bg::exterior_ring(epPolygon_i) = boost::assign::list_of<point_i>(3,10)(5,11)(4,14)(1,14)(1,13)(3,12);
    epNEP.resetPolygonForce(epPolygon_i);
    elementarypieces.push_back(epNEP);
    bg::exterior_ring(epPolygon_i) = boost::assign::list_of<point_i>(7,13)(10,11)(10,14)(7,14)(7,13);
    epNEP.resetPolygonForce(epPolygon_i);
    elementarypieces.push_back(epNEP);
    bg::exterior_ring(epPolygon_i) = boost::assign::list_of<point_i>(14,12)(16,12)(16,16)(13,16)(14,12);
    epNEP.resetPolygonForce(epPolygon_i);
    elementarypieces.push_back(epNEP);
    bg::exterior_ring(epPolygon_i) = boost::assign::list_of<point_i>(19,11)(24,11)(23,16)(20,15)(19,11);
    epNEP.resetPolygonForce(epPolygon_i);
    elementarypieces.push_back(epNEP);
    bg::exterior_ring(epPolygon_i) = boost::assign::list_of<point_i>(1,15)(3,15)(5,16)(1,16)(1,15);
    epNEP.resetPolygonForce(epPolygon_i);
    elementarypieces.push_back(epNEP);
    bg::exterior_ring(epPolygon_i) = boost::assign::list_of<point_i>(1,1)(5,1)(5,4)(2,4)(1,3)(1,1);
    epNEP.resetPolygonForce(epPolygon_i);
    elementarypieces.push_back(epNEP);
    bg::exterior_ring(epPolygon_i) = boost::assign::list_of<point_i>(7,1)(9,1)(9,3)(7,3)(7,1);
    epNEP.resetPolygonForce(epPolygon_i);
    elementarypieces.push_back(epNEP);
    bg::exterior_ring(epPolygon_i) = boost::assign::list_of<point_i>(13,1)(17,0)(17,3)(11,3)(13,1);
    epNEP.resetPolygonForce(epPolygon_i);
    elementarypieces.push_back(epNEP);
    bg::exterior_ring(epPolygon_i) = boost::assign::list_of<point_i>(1,6)(4,6)(4,10)(2,10)(1,6);
    epNEP.resetPolygonForce(epPolygon_i);
    elementarypieces.push_back(epNEP);
    bg::exterior_ring(epPolygon_i) = boost::assign::list_of<point_i>(6,7)(9,4)(9,10)(6,9)(6,7);
    epNEP.resetPolygonForce(epPolygon_i);
    elementarypieces.push_back(epNEP);
    bg::exterior_ring(epPolygon_i) = boost::assign::list_of<point_i>(13,4)(13,10)(11,10)(13,4);
    epNEP.resetPolygonForce(epPolygon_i);
    elementarypieces.push_back(epNEP);
    bg::exterior_ring(epPolygon_i) = boost::assign::list_of<point_i>(16,4)(19,4)(17,10)(16,7)(16,4);
    epNEP.resetPolygonForce(epPolygon_i);
    elementarypieces.push_back(epNEP);
    bg::exterior_ring(epPolygon_i) = boost::assign::list_of<point_i>(1,12)(6,12)(3,15)(1,15)(1,12);
    epNEP.resetPolygonForce(epPolygon_i);
    elementarypieces.push_back(epNEP);
    bg::exterior_ring(epPolygon_i) = boost::assign::list_of<point_i>(6,14)(8,14)(11,15)(11,17)(8,16)(6,16)(6,14);
    epNEP.resetPolygonForce(epPolygon_i);
    elementarypieces.push_back(epNEP);
    bg::exterior_ring(epPolygon_i) = boost::assign::list_of<point_i>(15,12)(18,12)(18,15)(19,18)(15,18)(15,12);
    epNEP.resetPolygonForce(epPolygon_i);
    elementarypieces.push_back(epNEP);
    //isplaced true
    bg::exterior_ring(epPolygon_i) = boost::assign::list_of<point_i>(0,0)(6,0)(6,1)(2,2)(0,2)(0,0);
    epNEP.resetPolygonForce(epPolygon_i);
    elementarypieces.push_back(epNEP);
    bg::exterior_ring(epPolygon_i) = boost::assign::list_of<point_i>(6,0)(9,0)(9,2)(6,2)(6,0);
    epNEP.resetPolygonForce(epPolygon_i);
    elementarypieces.push_back(epNEP);
    bg::exterior_ring(epPolygon_i) = boost::assign::list_of<point_i>(0,3)(4,4)(4,5)(0,5)(0,3);
    epNEP.resetPolygonForce(epPolygon_i);
    elementarypieces.push_back(epNEP);
    bg::exterior_ring(epPolygon_i) = boost::assign::list_of<point_i>(6,4)(8,4)(8,3)(10,3)(10,6)(6,6)(6,4);
    epNEP.resetPolygonForce(epPolygon_i);
    elementarypieces.push_back(epNEP);
    bg::exterior_ring(epPolygon_i) = boost::assign::list_of<point_i>(12,1)(15,2)(18,2)(17,4)(12,3)(12,1);
    epNEP.resetPolygonForce(epPolygon_i);
    elementarypieces.push_back(epNEP);
    bg::exterior_ring(epPolygon_i) = boost::assign::list_of<point_i>(19,1)(22,2)(25,2)(25,4)(19,3)(19,1);
    epNEP.resetPolygonForce(epPolygon_i);
    elementarypieces.push_back(epNEP);
    bg::exterior_ring(epPolygon_i) = boost::assign::list_of<point_i>(1,6)(3,6)(4,9)(0,9)(0,8)(1,6);
    epNEP.resetPolygonForce(epPolygon_i);
    elementarypieces.push_back(epNEP);
    bg::exterior_ring(epPolygon_i) = boost::assign::list_of<point_i>(6,7)(12,7)(12,9)(8,9)(6,8)(6,7);
    epNEP.resetPolygonForce(epPolygon_i);
    elementarypieces.push_back(epNEP);
    bg::exterior_ring(epPolygon_i) = boost::assign::list_of<point_i>(15,5)(16,9)(13,9)(15,5);
    epNEP.resetPolygonForce(epPolygon_i);
    elementarypieces.push_back(epNEP);
    bg::exterior_ring(epPolygon_i) = boost::assign::list_of<point_i>(18,4)(19,5)(20,9)(19,12)(17,12)(17,7)(18,4);
    epNEP.resetPolygonForce(epPolygon_i);
    elementarypieces.push_back(epNEP);
    bg::exterior_ring(epPolygon_i) = boost::assign::list_of<point_i>(20,5)(24,5)(25,7)(24,9)(20,5);
    epNEP.resetPolygonForce(epPolygon_i);
    elementarypieces.push_back(epNEP);
    bg::exterior_ring(epPolygon_i) = boost::assign::list_of<point_i>(21,9)(23,11)(24,14)(20,13)(20,11)(21,9);
    epNEP.resetPolygonForce(epPolygon_i);
    elementarypieces.push_back(epNEP);
    bg::exterior_ring(epPolygon_i) = boost::assign::list_of<point_i>(1,10)(3,10)(4,11)(4,13)(0,13)(1,10);
    epNEP.resetPolygonForce(epPolygon_i);
    elementarypieces.push_back(epNEP);
    bg::exterior_ring(epPolygon_i) = boost::assign::list_of<point_i>(6,10)(11,10)(11,12)(6,12)(6,10);
    epNEP.resetPolygonForce(epPolygon_i);
    elementarypieces.push_back(epNEP);
    bg::exterior_ring(epPolygon_i) = boost::assign::list_of<point_i>(12,10)(15,10)(14,16)(12,16)(13,14)(12,10);
    epNEP.resetPolygonForce(epPolygon_i);
    elementarypieces.push_back(epNEP);
    bg::exterior_ring(epPolygon_i) = boost::assign::list_of<point_i>(0,14)(4,14)(3,15)(2,15)(2,17)(0,17)(0,14);
    epNEP.resetPolygonForce(epPolygon_i);
    elementarypieces.push_back(epNEP);
    bg::exterior_ring(epPolygon_i) = boost::assign::list_of<point_i>(6,15)(9,13)(9,15)(6,17)(6,15);
    epNEP.resetPolygonForce(epPolygon_i);
    elementarypieces.push_back(epNEP);
    bg::exterior_ring(epPolygon_i) = boost::assign::list_of<point_i>(15,14)(18,14)(22,15)(22,16)(15,16)(15,14);
    epNEP.resetPolygonForce(epPolygon_i);
    elementarypieces.push_back(epNEP);
    bg::exterior_ring(epPolygon_i) = boost::assign::list_of<point_i>(1,0)(3,2)(2,4)(1,4)(1,0);
    epNEP.resetPolygonForce(epPolygon_i);
    elementarypieces.push_back(epNEP);
    bg::exterior_ring(epPolygon_i) = boost::assign::list_of<point_i>(5,1)(7,1)(6,6)(5,6)(4,4)(5,1);
    epNEP.resetPolygonForce(epPolygon_i);
    elementarypieces.push_back(epNEP);
    bg::exterior_ring(epPolygon_i) = boost::assign::list_of<point_i>(2,6)(3,6)(3,12)(2,12)(2,11)(1,8)(2,6);
    epNEP.resetPolygonForce(epPolygon_i);
    elementarypieces.push_back(epNEP);
    bg::exterior_ring(epPolygon_i) = boost::assign::list_of<point_i>(5,7)(10,8)(10,9)(6,10)(4,12)(5,7);
    epNEP.resetPolygonForce(epPolygon_i);
    elementarypieces.push_back(epNEP);

    field.setElementaryPieces(elementarypieces);
    //isPlaced
    std::array<bool , 50> isPlaced;
    isPlaced.fill(false);
    for(int i = 24 ; i < 46 ; i++) isPlaced[i] = true;
    //field_frame
    polygon_i ffPolygon_i_1{{{2 , 2}, {5 , 1}, {7 , 2}, {11 , 2}, {9 , 6}, {6 , 5}, {3 , 7}, {1 , 6}, {2 ,2}}};
    polygon_i ffPolygon_i_2{{
            {3 , 9}, {6 , 7}, {11 , 8}, {15 , 8}, {15 , 10}, {13 , 10}, {13 , 14},
            {14 , 14}, {14 , 16}, {11 , 16}, {11 , 18}, {6 , 18}, {6 , 16}, {5 , 16},
            {5 , 14}, {1 , 14}, {1 , 13}, {3 , 13}, {3 , 9}
    }};
    polygon_i ffPolygon_i_3{{{16 , 0}, {25 , 0}, {25 , 8}, {20 , 7}, {18 , 7}, {17 , 3}, {16 , 2}, {16 , 0}}};
    polygon_i ffPolygon_i_4{{{21 , 10}, {25 , 9}, {25 , 18} , {19 , 18}, {19 , 12}, {21 , 10}}};

    procon::NeoExpandedPolygon ffNEP1;
    ffNEP1.resetPolygonForce(ffPolygon_i_1);
    procon::NeoExpandedPolygon ffNEP2;
    ffNEP2.resetPolygonForce(ffPolygon_i_2);
    procon::NeoExpandedPolygon ffNEP3;
    ffNEP3.resetPolygonForce(ffPolygon_i_3);
    procon::NeoExpandedPolygon ffNEP4;
    ffNEP4.resetPolygonForce(ffPolygon_i_4);

    field.setFrame({ffNEP1 , ffNEP2 , ffNEP3 , ffNEP4});

    return field;
}

bool TestCheckCanPrume::run(){
    procon::NeoField field;

    procon::NeoExpandedPolygon nep;
    polygon_i boostPolygon_i;
    std::vector<procon::NeoExpandedPolygon>frame;
    std::vector<procon::NeoExpandedPolygon>pieces;

    //ここからframe
    bg::exterior_ring(boostPolygon_i) = boost::assign::list_of<point_i>(0,0)(2,0)(2,2)(0,2)(0,0);
    bg::correct(boostPolygon_i);
    NeoPolygonViewer::getInstance().displayPolygon(boostPolygon_i,"Frame",false);
    nep.resetPolygonForce(boostPolygon_i);
    frame.push_back(nep);

    bg::exterior_ring(boostPolygon_i) = boost::assign::list_of<point_i>(0,0)(6,0)(6,3)(3,6)(0,0);
    bg::correct(boostPolygon_i);
    NeoPolygonViewer::getInstance().displayPolygon(boostPolygon_i,"Frame",false);
    nep.resetPolygonForce(boostPolygon_i);
    frame.push_back(nep);


    field.setFrame(frame);

    //ここからpiece
    bg::exterior_ring(boostPolygon_i) = boost::assign::list_of<point_i>(0,0)(2,0)(2,3)(0,3)(0,0);
    bg::correct(boostPolygon_i);
    NeoPolygonViewer::getInstance().displayPolygon(boostPolygon_i,"Piece",false);
    nep.resetPolygonForce(boostPolygon_i);
    pieces.push_back(nep);

    bg::exterior_ring(boostPolygon_i) = boost::assign::list_of<point_i>(0,0)(1,0)(1,2)(0,2)(0,0);
    bg::correct(boostPolygon_i);
    NeoPolygonViewer::getInstance().displayPolygon(boostPolygon_i,"Piece",false);
    nep.resetPolygonForce(boostPolygon_i);
    pieces.push_back(nep);

    bg::exterior_ring(boostPolygon_i) = boost::assign::list_of<point_i>(0,0)(1,0)(1,2)(0,2)(0,0);
    bg::correct(boostPolygon_i);
    NeoPolygonViewer::getInstance().displayPolygon(boostPolygon_i,"Piece",false);
    nep.resetPolygonForce(boostPolygon_i);
    pieces.push_back(nep);

    bg::exterior_ring(boostPolygon_i) = boost::assign::list_of<point_i>(0,0)(2,0)(0,2)(0,0);
    bg::correct(boostPolygon_i);
    NeoPolygonViewer::getInstance().displayPolygon(boostPolygon_i,"Piece",false);
    nep.resetPolygonForce(boostPolygon_i);
    pieces.push_back(nep);

    bg::exterior_ring(boostPolygon_i) = boost::assign::list_of<point_i>(7,0)(0,0)(7,3)(7,0);
    bg::correct(boostPolygon_i);
    NeoPolygonViewer::getInstance().displayPolygon(boostPolygon_i,"Piece",false);
    nep.resetPolygonForce(boostPolygon_i);
    pieces.push_back(nep);


    field.setElementaryPieces(pieces);


    //検査用にProbMakerからテストパターンを引っ張る
/*
    ProbMaker *PbMaker = new ProbMaker();
    PbMaker->show();
    std::vector<polygon_i> pieces_ = PbMaker->getPieces();
    polygon_i frame_ = PbMaker->getFrame();

    std::vector<procon::NeoExpandedPolygon> neo_pieces;
    for(auto piece_ : pieces_){
        procon::NeoExpandedPolygon buf;
        buf.resetPolygonForce(piece_);
        neo_pieces.push_back(buf);
    }
    std::vector<procon::NeoExpandedPolygon> neo_frame;
    procon::NeoExpandedPolygon buf;
    buf.resetPolygonForce(frame_);
    neo_frame.push_back(buf);

    field.setElementaryPieces(neo_pieces);
    field.setFrame(neo_frame);
*/

    //ここまでProbMakerからの部分　使わないならコメントアウトで


    BeamSearch beamsearch;
    bool a = beamsearch.checkCanPrune(field);
    std::string str =( a
                      ?"枝切りが可能です(矛盾があります)"
                      :"枝切りできません(問題はないです)");
    std::cout << "結果　：　"<< str << std::endl;
    return true;
}
