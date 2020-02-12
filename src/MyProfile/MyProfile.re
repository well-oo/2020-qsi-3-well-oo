open ReasonUrql;
open Hooks;
module GetProfile = [%graphql
  {|
    query{
        viewer{
            name
            avatarUrl
            status{
                emojiHTML
                message
            }
            followers{
                totalCount
            }
            following   {
                totalCount
            }
        }
    }   
|}
];

let renderEmojiHTML(emojiHTML){
{switch (Js.Json.decodeString(emojiHTML)) {
                        //| Some(emojiHTML) => <div dangerouslySetInnerHTML={ "__html": emojiHTML } />
                        |Some(emojiHTML) => {
                            let matches = Js.String.match([%re "/\"(.*?)\"/g"], emojiHTML);
                            {switch (matches) {
                            | Some(matches) => {
                                let alt = ref(matches[1]);
                                let src = ref(matches[2]);
                                if(Array.length(matches) > 3){
                                     alt := matches[2];
                                     src := matches[3];
                                };
                                <img alt=Js.String.replaceByRe([%re "/\"/g"],"",alt^) height="20" width="20" src=Js.String.replaceByRe([%re "/\"/g"],"",src^) />
                            }
                            | None => React.null
                            }}
                        }
                        | None => React.null
                    }};
};

[@react.component]
let make = () => {
let request = GetProfile.make();
let ({response}, _) = useQuery(~request, ());
  switch (response) {
    | Fetching => <div> "Fetching ..."->React.string </div>
    | Data(data) => {
        switch (data##viewer) {
        | viewer =>
        switch (
            viewer##name,
            Js.Json.decodeString(viewer##avatarUrl),
            viewer##status,
            viewer##followers,
            viewer##following
        ) {
        | (
            Some(name),
            Some(avatarUrl),
            status,
            followers,
            following
            ) =>
            <section>
            <div>
                <img src=avatarUrl width="200px"/>
                <p>name->React.string</p>
                {switch (status) {
               | Some(status) => {
                   switch (status##emojiHTML, status##message) {
                       | (Some(emojiHTML), Some(message)) =>
                        <div>
                        {emojiHTML |> renderEmojiHTML}
                        message->React.string
                        </div>
                        | (None, Some(message)) => <div>message->React.string</div>
                        | (Some(emojiHTML), _) => <div>
                        {emojiHTML |> renderEmojiHTML}
                        </div>
                        | (None, None) => React.null
                   }
                }
               | (_) => React.null
               }}
               <p>(string_of_int(followers##totalCount) ++ " followers / " ++ string_of_int(following##totalCount) ++ " following")->React.string</p>
            </div>
            </section>
        | (_, _,_,_,_) => React.null
        }
        }
    }
    | Error(e) =>
      switch (e.networkError) {
      | Some(_exn) => <div> "Network Error"->React.string </div>
      | None => <div> "Other Error"->React.string </div>
      }
    | NotFound => <div> "Something went wrong!"->React.string </div>
  }
}