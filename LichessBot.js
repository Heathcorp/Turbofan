const fs = require("fs");
const fetch = require('node-fetch');
const readline = require('readline');

const personalToken = JSON.parse(fs.readFileSync("./token.json")); //secret lichess api token, token.json is ignored by git

var currentGames = [];
var currentChallenges = [];

function Main() {
    fetch('https://lichess.org/api/stream/event', {
        method: "get",
        headers: {
            'Authorization': 'Bearer ' + personalToken
        }
    }).then(response => {
        const responseReader = readline.createInterface({input: response.body});
        responseReader.on("line", line => {
            if (line != "") {
                let lichessEvent = JSON.parse(line);
                console.log(lichessEvent);
                if (lichessEvent.type == "challenge") {
                    currentChallenges[lichessEvent.challenge.id] = lichessEvent.challenge;
                } else if (lichessEvent.type == "challengeCanceled") {
                    delete currentChallenges[lichessEvent.challenge.id];
                } else if (lichessEvent.type == "challengeDeclined") {
                    delete currentChallenges[lichessEvent.challenge.id];
                } else if (lichessEvent.type == "gameStart") {
                    //trigger game streaming and playing
                } else if (lichessEvent.type == "gameFinish") {
                    
                }
            }
        });
    }).catch(error => {
        console.log(error);
    });
}

function GetGames() {
    fetch('https://lichess.org/api/account/playing', {
        method: "get",
        headers: {
            'Authorization': 'Bearer ' + personalToken
        }
    }).then(response => response.json()).then(response => {
        currentGames = Array(response.nowPlaying.length);
        for (i = 0; i < response.nowPlaying.length; i++) {
            currentGames[i] = response.nowPlaying[i];
        }
    }).catch(error => {
        console.log(error);
    });
}

function MakeMove(gameId, move) {
    fetch('https://lichess.org/api/bot/game/' + gameId + "/move/" + move, {
        method: "post",
        headers: {
            'Authorization': 'Bearer ' + personalToken
        }
    }).then(response => response.json()).then(response => {
        console.log("Played " + move + " in game " + gameId);
    }).catch(error => {
        console.log(error);
    });
}