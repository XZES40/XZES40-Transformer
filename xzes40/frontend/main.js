///////////////////////////////////////////////////////////////////////////////
// Copyright 2017, the Oregon State University Capstone Project 'XZES40'
// with the Apache Software Foundation
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
///////////////////////////////////////////////////////////////////////////////

$(function(){
    var faces = {};
    faces.good = ["&#x1F60E;", "&#x1F60A;", "&#x1F604;", "&#x1F601;"];
    faces.bad  = ["&#x1F625;", "&#x1F622;", "&#x1F61E;", "&#x1F61F;",
                  "&#x1F623;", "&#x1F628;", "&#x1F629;", "&#x1F62B;",
                  "&#x1F62D;", "&#x1F630;", "&#x1F633;", "&#x1F635;",
                  "&#x1F616;", "&#x1F613;", "&#x1F615;"];
    var getFace = function(emotion) {
        n = Math.floor(Math.random() * 100) % faces[emotion].length;
        return faces[emotion][n];
    }
    var updateStatus = function(msg) {
        $("div#result").html(msg);
    }
    var notSuccessful = function(content) {
        updateStatus(content + " " + getFace("bad"));
        $("div#viewDoc").html("");
    }
    var successful = function(content) {
        /* Output Message */
        var m = "";
        var date = new Date();
        var filename = date.getFullYear() + '-'
                     + date.getMonth()    + '-' 
                     + date.getDate()     + 'T'
                     + date.getHours()    + ':'
                     + date.getMinutes()  + ':'
                     + date.getSeconds()  + '.xml';

        m += "<p>Transformation Successful! " + getFace("good") + "</p>";
        m += "<p>" + filename + "</p>";
        /* Test if FileSaver works on this browser */
        try {
            /* If it is supported, add the download button */
            var isFileSaverSupported = !!new Blob;
            m += "<button type=\"button\" id=\"downloadXML\">Download Output</button>";
        } catch (e) {
            /* If it is not there, tell the user why they can't download the file */
            m += "Your browser does not support downloading transformed XML files.";
            m += "Consider upgrading your browser.";
            m += "<br>";
        }
        /* Always allow the user to just view the file */
        m += "<button type=\"button\" id=\"viewXML\">View Raw XML</button>";
        updateStatus(m);
        $("#viewXML").click(function(event){
            $("div#viewDoc").html("<textarea readonly name=\"\" cols=\"\" rows=\"\"> " + content + "</textarea>");
        });
        $("#downloadXML").click(function(event){
            var blob= new Blob([content], {type: "text/plain;charset=utf-8"});
            console.log(blob);
            saveAs(blob, filename);
        });
    }
    var processParameters = function() {
        var p = {};
        var s = $("#parameterValues").children();
        console.log( s );

        for (i=0; i<s.length; i+=3) {
            // {key: value}
            // i+=3 to avoid <hr> parsing
            if ($(s[i]).val() && $(s[i+1]).val()) {
                p[$(s[i]).val()] = $(s[i+1]).val();
            }
        }
        return p;
    }

    $("input#addParameters").click(function(event) {
        $("#parameterValues").append("<input type=\"text\" id=\"key\" placeholder=\"Key\"><input type=\"text\" id=\"value\" placeholder=\"Value\"><br/>");
    });
    $("form#transform").submit(function(event) {
        event.preventDefault();

        var file = "";
        var formData = new FormData($(this)[0]);
        formData.append("parameters", JSON.stringify(processParameters()));
        console.log( formData.get("parameters") );

        $.ajax({
            url: "/cgi-bin/xzes.py",
            type: "POST",
            data: formData,
            async: true,
            statusCode: {
                400: function(data) {
                    notSuccessful(data.responseText);
                },
                200: function(data) {
                    if (data.responseText != ""
                     && data.responseText != undefined) {
                        successful(data.responseText);
                    } else {
                        notSuccessful("The transformation was not successful...");
                    }
                },
            },
            cache: false,
            contentType: false,
            processData: false,
        });
        console.log(file);
    });
});
