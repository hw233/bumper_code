var fs = require("fs");

var filenames = ["GameScene", "DLDGameScene", "DQZGameScene"];
var savenames = ["PracticeCarPosTable", "DLDCarPosTable", "DQZCarPosTable"];

for(var i = 0;i < filenames.length;i ++)
{
    var saveData = [];

    var item = filenames[i];
    var fileName = "assets/Scene/" + item + ".fire";
    var fb = fs.openSync(fileName, "r+");
    var data = fs.readFileSync(fileName)
    var jsonData = JSON.parse(data.toString());
    for(var j = 0;j < jsonData.length;j ++)
    {
        var tempData = jsonData[j];
        if(tempData && tempData["_name"] === "_CarPos_" && tempData["_position"])
        {
            var pos = {
                x: tempData["_position"].x,
                y: tempData["_position"].y,
            }

            saveData.push(pos);
        }
    }

    var savename = "assets/Script/table/" + savenames[i] + ".ts";

    var savedata = "export const " + savenames[i] +" = " + JSON.stringify(saveData) + ";";

    fs.writeFileSync(savename, savedata);

    fs.closeSync(fb);
}
