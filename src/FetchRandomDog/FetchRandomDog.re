[@bs.val] external fetch: string => Js.Promise.t('a) = "fetch";

type state =
  | LoadingDogs
  | ErrorFetchingDogs
  | LoadedDogs(array(string));

let add_dog = setState => {
  Js.Promise.(
      fetch("https://dog.ceo/api/breeds/image/random/1")
      |> then_(response => response##json())
      |> then_(jsonResponse => {
           setState(_previousState => LoadedDogs(jsonResponse##message));
           Js.Promise.resolve();
         })
      |> catch(_err => {
           setState(_previousState => ErrorFetchingDogs);
           Js.Promise.resolve();
         })
      |> ignore
    );
};

[@react.component]
let make = () => {
  let (state, setState) = React.useState(() => LoadingDogs);



  // Notice that instead of `useEffect`, we have `useEffect0`. See
  // reasonml.github.io/reason-react/docs/en/components#hooks for more info
  React.useEffect0(() => {
    add_dog(setState)

    // Returning None, instead of Some(() => ...), means we don't have any
    // cleanup to do before unmounting. That's not 100% true. We should
    // technically cancel the promise. Unofortunately, there's currently no
    // way to cancel a promise. Promises in general should be way less used
    // for React components; but since folks do use them, we provide such an
    // example here. In reality, this fetch should just be a plain callback,
    // with a cancellation API
    None;
  });
  <>
    <div
      style={ReactDOMRe.Style.make(
        ~height="120px",
        ~display="flex",
        ~alignItems="center",
        ~justifyContent="center",
        (),
      )}>
      {switch (state) {
       | ErrorFetchingDogs => React.string("An error occurred!")
       | LoadingDogs => React.string("Loading...")
       | LoadedDogs(dogs) =>
         dogs
         ->Belt.Array.mapWithIndex((i, dog) => {
             let imageStyle =
               ReactDOMRe.Style.make(
                 ~height="120px",
                 ~display="flex",
                 ~width="30%",
                 ~marginRight=i === Js.Array.length(dogs) - 1 ? "0px" : "8px",
                 ~borderRadius="8px",
                 ~boxShadow="0px 4px 16px rgb(200, 200, 200)",
                 ~backgroundSize="cover",
                 ~backgroundImage={j|url($dog)|j},
                 ~backgroundPosition="center",
                 (),
               );
             <div key=string_of_int(i) style=imageStyle />;
           })
         ->React.array
       }}
    </div>
    <button onClick={_ => add_dog(setState)}> {React.string("New dog")} </button>
  </>;
};