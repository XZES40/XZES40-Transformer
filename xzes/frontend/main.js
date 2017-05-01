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
    // A cute little easter egg.
    var faces = {good: ["&#x1F60E;", "&#x1F60A;", "&#x1F604;", "&#x1F601;"],
                 bad:  ["&#x1F625;", "&#x1F622;", "&#x1F61E;", "&#x1F61F;", "&#x1F623;", "&#x1F628;", "&#x1F629;", "&#x1F62B;", "&#x1F62D;", "&#x1F630;", "&#x1F633;", "&#x1F635;", "&#x1F616;", "&#x1F613;", "&#x1F615;"]};
    // Gets a random face from the above json object.
    var getFace = function(emotion) {
        n = Math.floor(Math.random() * 100) % faces[emotion].length;
        return faces[emotion][n];
    }
    // Updates the text in the status div.
    var updateStatus = function(msg) {
        $("div#result").html(msg);
    }
    // Displays an uncuccessful status.
    // Clears any previous successful status text.
    var notSuccessful = function(content) {
        updateStatus(content + " " + getFace("bad"));
        $("div#viewDoc").html("");
    }
    // Generates a filename based on the current date
    var generateFilename = function() {
        var date = new Date();
        return date.getFullYear() + '-'
             + date.getMonth()    + '-' 
             + date.getDate()     + 'T'
             + date.getHours()    + ':'
             + date.getMinutes()  + ':'
             + date.getSeconds()  + '.xml';
    }

    // returns a string about the status of the browser.
    // This is appended to a success status.
    var isDownloadSupported = function() {
        /* Test if FileSaver works on this browser */
        try {
            /* If it is supported, add the download button */
            var isFileSaverSupported = !!new Blob;
            return "<button type=\"button\" id=\"downloadXML\">Download Output</button>";
        } catch (e) {
            /* If it is not there, tell the user why they can't download the file */
            m = "<p>Your browser does not support downloading transformed XML files.</p>";
            m += "<p>Consider upgrading your browser.</p>";
            m += "<br>";
            return m
        }
    }
    // Populates the status section with appropriate status text
    var successful = function(content) {
        /* Output Message */
        var m = "";
        m += "<p>Transformation Successful! " + getFace("good") + "</p>";
        filename = generateFilename();
        m += "<p>" + filename + "</p>";
        m += isDownloadSupported();
        /* Always allow the user to just view the file */
        m += "<button type=\"button\" id=\"viewXML\">View Raw XML</button>";
        updateStatus(m);
        $("#viewXML").click(function(event){
            $("div#viewDoc").html("<textarea readonly name=\"\" cols=\"\" rows=\"\"> " + content + "</textarea>");
        });
        $("#downloadXML").click(function(event){
            var blob= new Blob([content], {type: "text/plain;charset=utf-8"});
            saveAs(blob, filename);
        });
    }
    // Processes the parameters in the form into JSON.
    // Return the JSON
    var processParameters = function() {
        var p = {};
        var s = $("#parameterValues").children();

        for (i=0; i<s.length; i+=3) {
            // {key: value}
            // i+=3 to avoid <hr> parsing
            if ($(s[i]).val() && $(s[i+1]).val()) {
                p[$(s[i]).val()] = $(s[i+1]).val();
            }
        }
        return p;
    }

    // Append a empty parameter to the form
    $("input#addParameters").click(function(event) {
        $("#parameterValues").append("<input type=\"text\" id=\"key\" placeholder=\"Key\"><input type=\"text\" id=\"value\" placeholder=\"Value\"><br/>");
    });

    // When happens when you submit the form
    $("form#transform").submit(function(event) {
        event.preventDefault();

        var file = "";
        var formData = new FormData($(this)[0]);
        formData.append("parameters", JSON.stringify(processParameters()));

        // Send the modified form to the /cgi-bin/xzes.py endpoint.
        // If you messed with the apache service this will need to change.
        $.ajax({
            url: "/cgi-bin/xzes.py",
            type: "POST",
            data: formData,
            dataType: "text",
            async: true,
            statusCode: {
                404: function(data) {
                    notSuccessful("&#x1F6AB; &#x2601; Could not reach the transformer server...");
                },
                400: function(data) {
                    notSuccessful(data.responseText);
                },
                // On successful transformation
                200: function(data) {
                    // If the repsonse is non-empty
                    if (data.responseText != ""
                     && data.responseText != undefined) {
                        // Print the success, the filename, and give buttons to view/downlaod the file.
                        successful(data.responseText);
                    } else if (typeof(data) == "string") {
                        // The response may just be a string...
                        // We're not sure why this happens sometimes...
                        successful(data);
                    } else {
                        // Otherwise give a generic failure message
                        // Weird edge case where output xml is parsed by jquery as html
                        notSuccessful("The transformation was not successful.");
                    }
                },
            },
            cache: false,
            contentType: false,
            processData: false,
        });
    });
});
