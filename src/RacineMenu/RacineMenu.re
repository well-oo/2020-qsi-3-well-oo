[@react.component]
let make = () => {
  let url = ReasonReactRouter.useUrl();

  let li = ReactDOMRe.Style.make(~cursor="pointer", ());
  
<div>
    <ul style=(li)>
        <li onClick={_ => ReasonReactRouter.push("/greeting")}>{React.string("Atomic Greeting")}</li>
        <li onClick={_ => ReasonReactRouter.push("/blinking")}>{React.string("Blinking Greeting")}</li>
        <li onClick={_ => ReasonReactRouter.push("/reducer")}>{React.string("Reducer From ReactJS Docs")}</li>
        <li onClick={_ => ReasonReactRouter.push("/fetched")}>{React.string("Fetched Dog Pictures")}</li>
        <li onClick={_ => ReasonReactRouter.push("/random")}>{React.string("1 random Dog")}</li>
        <li onClick={_ => ReasonReactRouter.push("/profile")}>{React.string("My Github profile")}</li>
        <li onClick={_ => ReasonReactRouter.push("/status")}>{React.string("Modify my status")}</li>
    </ul>
{
    switch (url.path) {
    | ["greeting"] => <Greeting />;
    | ["blinking"] => <BlinkingGreeting> {React.string("Ayaaaa!")} </BlinkingGreeting>;
    | ["reducer"] => <ReducerFromReactJSDocs />;
    | ["fetched"] => <FetchedDogPictures />;
    | ["random"] => <FetchRandomDog />;
    | ["profile"] => <MyProfile />;
    | ["status"] => <Status />;
    | _ => <div>{React.string("Cliquer sur un lien")}</div>
    };
}
</div>
}