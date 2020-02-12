 [@bs.val] external alert : string => unit = "alert";
 open ReasonUrql;
 open Hooks;
 

 module GetProfile = [%graphql
  {|
    query{
        viewer{
            status{
                emoji
                message
            }
        }
    }   
|}
];

 module ChangeUserStatus = [%graphql
  {|
    mutation changeUserStatus($message: String!) {
      changeUserStatus(input: {
    message: $message,
  }) {
        status{
            message
        }
      }
    }
  |}
];


[@react.component]
let make = _ => {

let (message, setMessage) = React.useState(() => "");
let (emoji, setEmoji) = React.useState(() => "");

let init = () : unit => {
  let request = GetProfile.make();
  let ({response}, _) = useQuery(~request, ());
    switch (response) {
      | Data(data) => {
          switch (data##viewer) {
          | viewer =>
          switch (
              viewer##status
          ) {
          | (
              status
              ) =>
                  {switch (status) {
                | Some(status) => {
                    switch (status##emoji, status##message) {
                        | (Some(emoji), Some(message)) =>
                            setMessage((_) => message);
                            setEmoji((_) => emoji);
                        | (None, Some(message)) =>
                            setMessage((_) => message);
                        | (Some(emoji), None) =>
                            setEmoji((_) => emoji);
                        | (_, _) => print_endline("");
                    }
                  }
                | (_) => print_endline("");
                }}
          }
          }
      }
      | _ => print_endline("");
    }
};

  React.useEffect0(() => {
    //init();
    None;
  });



  let handleChange = e => {
  setMessage(e->ReactEvent.Form.target##value);
};

let (_, executeMutation) = useDynamicMutation(ChangeUserStatus.definition);

let handleSubmit = e => {
  e->ReactEvent.Form.preventDefault;
  let promise = executeMutation(
          ~message=message,
          ()
        );
  promise
|> Js.Promise.then_(_ => {
       alert("Updated !!");
       Js.Promise.resolve();
   })
|> Js.Promise.catch(_ => {
     alert("Error !!");
     Js.Promise.resolve();
   }) 
|> ignore;
};

  <div>
    <form onSubmit={handleSubmit}>
      <label htmlFor="status">"Status: "->React.string</label>
      <input id="status" value={message} name="status" onChange={handleChange} />
      <input type_="submit" value="Submit"/> 
    </form>
  </div>;
};