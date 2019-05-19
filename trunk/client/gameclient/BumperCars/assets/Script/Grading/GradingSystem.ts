import {GradingTable} from "./GradingTable";


export const GradingSystem = {
    //根据名次和段位ID获得是否加星或减星
    GetStarNum : function(rank, gradingID){
        if(gradingID > 31){
            gradingID = 31;
        }

        let gradingInfo = GradingTable[gradingID];

        if(!gradingInfo){
            return 0;
        }

        //加两星
        let plus2StarRange:any = gradingInfo.PlusStar2;
        if(plus2StarRange){
            plus2StarRange = plus2StarRange.split("~");
            if(plus2StarRange.length == 2){
                if(rank >= plus2StarRange[0] && rank <= plus2StarRange[1]){
                    return 2;
                }
            }else if(plus2StarRange.length == 1){
                if(rank == plus2StarRange[0]){
                    return 2;
                }
            }
        }


        //加星的名次范围
        let plusStarRange:any = gradingInfo.PlusStar;
        if(plusStarRange){
            plusStarRange = plusStarRange.split("~");

            if(plusStarRange.length == 2){
                if(rank >= plusStarRange[0] && rank <= plusStarRange[1]){
                    return 1;
                }
            }else if(plusStarRange.length == 1){
                if(rank == plusStarRange[0]){
                    return 1;
                }
            }
        }

        //不加不减的名次范围
        let flatStarRange:any = gradingInfo.FlatStar;
        if(flatStarRange){
            flatStarRange = flatStarRange.split("~");

            if(flatStarRange.length == 2){
                if(rank >= flatStarRange[0] && rank <= flatStarRange[1]){
                    return 0;
                }
            }else if(flatStarRange.length == 1){
                if(rank == flatStarRange[0]){
                    return 0;
                }
            }
        }
        
        //减星的名次范围
        let dropStarRange:any = gradingInfo.DropStar;
        if(dropStarRange){
            dropStarRange = dropStarRange.split("~");

            if(dropStarRange.length == 2){
                if(rank >= dropStarRange[0] && rank <= dropStarRange[1]){
                    return -1;
                }
            }else if(dropStarRange.length == 1){
                if(rank == dropStarRange[0]){
                    return -1;
                }
            }
        }
        
        return 0;
    },

    //根据段位ID获得段位的文字描述
    GetGradingName: function(gradingID){
        if(gradingID > 31){
            gradingID = 31;
        }

        let gradingInfo = GradingTable[gradingID];

        if(!gradingInfo){
            return "";
        }

        return gradingInfo.Paragraph;
    },

    //根据段位ID获得段位文件名
    GetGradingFileName: function(gradingID){
        if(gradingID > 31){
            gradingID = 31;
        }
        let gradingInfo = GradingTable[gradingID];

        if(!gradingInfo){
            return "";
        }

        return gradingInfo.FileName;
    }
};