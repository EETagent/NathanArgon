mod cargon;

use serde::{Deserialize, Serialize};
use tide::{Body, Request};

#[derive(Deserialize)]
struct AppRequest { plain: String }
#[derive(Serialize)]
struct AppResponse { hash: String }

#[async_std::main]
async fn main() -> tide::Result<()> {

    let mut app = tide::new();

    app.at("/hash").post(|mut req: Request<()>| async move {
        let request: AppRequest = req.body_json().await?;
        let argon = cargon::run_hash(100, 4096, 1, &request.plain, "nejakysalt").await;
        let response = AppResponse {
            hash: cargon::run_decode(argon.encoded).await
        };

        Body::from_json(&response)
    });
    app.listen("0.0.0.0:80").await?;
    Ok(())
}